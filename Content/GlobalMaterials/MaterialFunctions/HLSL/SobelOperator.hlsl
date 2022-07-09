void CalculateSobelGradient(float3 SceneTexture, float2 UV, float TexelSize, out float2 Gradient) {
    Gradient = float2(0, 0);

    float sobelX[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    float sobelY[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

    int index = 0;

    for(int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (index == 4) {
                index++;
                continue;
            }
            float2 offset = float2(x, y) * TexelSize;
            float3 pxCol = SceneTextureLookup(UV + offset, 14, false).xyz;
            float pxLum = dot(pxCol, float3(0.2126, 0.7152, 0.0722));

            Gradient.x += pxLum * sobelX[index];
            Gradient.y += pxLum * sobelY[index];

            index++;
        }
    }
}

void CalculateDepthSobelGradient(float3 SceneTexture, float2 UV, float TexelSize, out float2 Gradient) {
    Gradient = float2(0, 0);

    float sobelX[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    float sobelY[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

    int index = 0;

    for(int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (index == 4) {
                index++;
                continue;
            }
            float2 offset = float2(x, y) * TexelSize;
            float3 pxCol = SceneTextureLookup(UV + offset, 1, false).xyz;
            float pxLum = dot(pxCol, float3(0.2126, 0.7152, 0.0722));

            Gradient.x += pxLum * sobelX[index];
            Gradient.y += pxLum * sobelY[index];

            index++;
        }
    }
}