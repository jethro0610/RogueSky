// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/DistanceField.h"

DistanceField::DistanceField() {
    // Initialize the distance field to the max distance
    for (int8 x = 0; x < DISTANCE_FIELD_SIZE; x++)
    for (int8 y = 0; y < DISTANCE_FIELD_SIZE; y++)
    for (int8 z = 0; z < DISTANCE_FIELD_SIZE; z++) {
        compressedDistanceField[x][y][z] = CHUNK_SIZE;
    }
}

DistanceField::~DistanceField() {

}

void DistanceField::SetDistance(int8 X, int8 Y, int8 Z, FFloat16 Distance) { 
    compressedDistanceField[X][Y][Z] = FMath::Clamp(Distance, (FFloat16)-CHUNK_SIZE, (FFloat16)CHUNK_SIZE);
}

FFloat16 DistanceField::GetDistance(int8 X, int8 Y, int8 Z, uint8 LODLevel) const {
    if (LODLevel == 0)
        return GetDistance(X, Y, Z);

    const uint8 lodUnitSize = FMath::Exp2(LODLevel);
    return GetDistance(X * lodUnitSize, Y * lodUnitSize, Z * lodUnitSize);
}

FVector DistanceField::GetNormal(int8 X, int8 Y, int8 Z) const {
    FFloat16 distances[8];
    for (int8 i = 0; i < 8; i++) {
        distances[i] = GetDistance(FIntVector8(X, Y, Z) + cornerTable[i]);
    }

    FFloat16 nX = (distances[1] - distances[0]) + (distances[2] - distances[3]) + (distances[5] - distances[4]) + (distances[6] - distances[7]);
    FFloat16 nY = (distances[3] - distances[0]) + (distances[2] - distances[1]) + (distances[7] - distances[4]) + (distances[6] - distances[5]);
    FFloat16 nZ = (distances[4] - distances[0]) + (distances[5] - distances[1]) + (distances[6] - distances[2]) + (distances[7] - distances[3]);

    return FVector(nX, nY, nZ).GetSafeNormal();
}

FVector DistanceField::GetNormal(int8 X, int8 Y, int8 Z, uint8 LODLevel) const {
    if (LODLevel == 0)
        return GetNormal(X, Y, Z);

    const uint8 lodUnitSize = FMath::Exp2(LODLevel);
    return GetNormal(X * lodUnitSize, Y * lodUnitSize, Z * lodUnitSize);
}

void DistanceField::AddDistanceField(DistanceField* DistanceFieldToAdd) {
    for (int8 x = 0; x < DISTANCE_FIELD_SIZE; x++)
    for (int8 y = 0; y < DISTANCE_FIELD_SIZE; y++)
    for (int8 z = 0; z < DISTANCE_FIELD_SIZE; z++) {
        FFloat16 min = FMath::Min(GetDistance(x, y, z), DistanceFieldToAdd->GetDistance(x, y, z));
        SetDistance(x, y, z, min);
    }
}