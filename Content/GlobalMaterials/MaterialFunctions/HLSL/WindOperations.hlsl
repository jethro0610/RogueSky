void GetWindVelocity(float2 UV, int Samples, float SampleDistance, float DeltaTime, Texture2D WindBuffer, out float2 Velocity) {
    Velocity = Texture2DSample(WindBuffer, WindBufferSampler, UV).rg;
    for (int x = -Samples; x <= Samples; x++)
    for (int y = -Samples; y <= Samples; y++) {
        if (x == 0 && y == 0)
            continue;

        float2 offset = float2(x, y) * (1.0f / SampleDistance);
        float2 bufferVelocity = Texture2DSample(WindBuffer, WindBufferSampler, UV + offset).rg;

        float2 directionToFieldPoint = float2(-x, -y);
        directionToFieldPoint = normalize(directionToFieldPoint);
        float velocityDot = dot(directionToFieldPoint, normalize(bufferVelocity));

        velocityDot = saturate((velocityDot + 1.0f) * 0.5f);
        velocityDot = pow(velocityDot, 32.0);
        Velocity += bufferVelocity * velocityDot * DeltaTime;
    }
    Velocity = Velocity * pow(0.8, DeltaTime * 60.0f);
}