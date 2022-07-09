void KuwaharaPostProcess(float3 SceneTexture, float2 Radius, float2 UV, float2 TexelSize, float2 Gradient, out float3 Color) {
    // Calculate the angle of the pixel from the gradient and create a matrix for rotation
    float pixelAngle = 0;
    if (abs(Gradient.x) > 0.001)
        pixelAngle = atan(Gradient.y / Gradient.x);
    float2x2 rotationMatrix = float2x2(cos(pixelAngle), -sin(pixelAngle), sin(pixelAngle), cos(pixelAngle));
    
    // Stores the mean color of each kernel quadrant
    float3 kernelMean[4] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    // Stores the standard deviation of each kernel quadrant
    float3 kernelVariance[4] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    // The ranges for each kernel quadrant
    float4 kernelRanges[4] = {
        {-Radius.x, 0, -Radius.y, 0},
        {0, Radius.x, -Radius.y, 0},
        {-Radius.x, 0, 0, Radius.y},
        {0, Radius.x, 0, Radius.y}
    };

    // Loop through each kernel
    for (int i = 0; i < 4; i++) {
        float totalSamples = 0; // Track the total samples so the mean can be calculated
        for (int j = kernelRanges[i].x; j <= kernelRanges[i].y; j++) { // Use the defined kernel ranges to sample the pixels in each kernel
            for (int k = kernelRanges[i].z; k <= kernelRanges[i].w; k++) {
                // Get the pixel's color for the kernel and add to the sums for variance and mean
                float2 offset = mul(float2(j, k) * TexelSize, rotationMatrix);
                float3 pixelColor = SceneTextureLookup(UV + offset, 14, false);
                kernelMean[i] += pixelColor;
                kernelVariance[i] += pixelColor * pixelColor;
                totalSamples++;
            }
        }
        // Get the variance and mean by dividing by the total by the total samplees
        kernelMean[i] /= totalSamples;
        kernelVariance[i] = abs(kernelVariance[i] / totalSamples - kernelMean[i] * kernelMean[i]);
    }

    // Find the kernel with the lowest variance and output the mean of that kernel
    float lowestVariance = 1;
    for(int i = 0; i < 4; i++) {
        float totalVariance = kernelVariance[i].r + kernelVariance[i].g + kernelVariance[i].b;

        if (totalVariance < lowestVariance) {
            lowestVariance = totalVariance;
            Color = kernelMean[i];
        }
    }
}