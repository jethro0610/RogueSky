void MultiLerp3(float Alpha, float3 Input1, float3 Input2, float3 Input3, out float3 Output) {

    float3 floatArray[3];
    floatArray[0] = Input1;
    floatArray[1] = Input2;
    floatArray[2] = Input3;

    int index = min(floor(Alpha * 3), 1);

    float x = lerp(floatArray[index].x, floatArray[index + 1].x, clamp((Alpha * 3) - index, 0, 1));
    float y = lerp(floatArray[index].y, floatArray[index + 1].y, clamp((Alpha * 3) - index, 0, 1));
    float z = lerp(floatArray[index].z, floatArray[index + 1].z, clamp((Alpha * 3) - index, 0, 1));

    Output = float3(x, y, z);
}

void MultiLerp4(float Alpha, float3 Input1, float3 Input2, float3 Input3, float3 Input4, out float3 Output) {

    float3 floatArray[4];
    floatArray[0] = Input1;
    floatArray[1] = Input2;
    floatArray[2] = Input3;
    floatArray[3] = Input4;
    int index = min(floor(Alpha * 4), 1);

    float x = lerp(floatArray[index].x, floatArray[index + 1].x, clamp((Alpha * 4) - index, 0, 1));
    float y = lerp(floatArray[index].y, floatArray[index + 1].y, clamp((Alpha * 4) - index, 0, 1));
    float z = lerp(floatArray[index].z, floatArray[index + 1].z, clamp((Alpha * 4) - index, 0, 1));

    Output = float3(x, y, z);
}