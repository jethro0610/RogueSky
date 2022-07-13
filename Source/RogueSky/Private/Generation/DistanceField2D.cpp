// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/DistanceField2D.h"

DistanceField2D::DistanceField2D() {
    // Initialize the distance field to the max distance
    for (int8 x = 0; x < DISTANCE_FIELD_SIZE; x++)
    for (int8 y = 0; y < DISTANCE_FIELD_SIZE; y++) {
        compressedDistanceField[x][y] = CHUNK_SIZE;
    }
}

DistanceField2D::~DistanceField2D() {

}

void DistanceField2D::SetDistance(int8 X, int8 Y, FFloat16 Distance) { 
    compressedDistanceField[X][Y] = Distance; 
}
