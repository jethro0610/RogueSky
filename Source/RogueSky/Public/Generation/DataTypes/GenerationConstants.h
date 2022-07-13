#pragma once
#include "Generation/GenerationTypes.h"
namespace GenerationConstants {
    // Table of cube corners
    const FIntVector8 cornerTable[8] = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 1, 0},
        {0, 1 ,0},
        {0, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 1, 1}
    };

    // Table of cube edges
    const uint8 edgeTable[12][2] = {
        {0, 1},
        {3, 2},
        {4, 5},
        {7, 6},

        {0, 3},
        {1, 2},
        {4, 7},
        {5, 6},

        {0, 4},
        {3, 7},
        {1, 5},
        {2, 6}
    };

    // Table of edges to check for triangulation
    const FIntVector8 triangleEdgeTable[3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    // Table of stitch
    const FIntVector8 stitchTable[3] = {
        {-1, 0, 0},
        {0, -1, 0},
        {0, 0, -1}
    };

    /*  Table will always generate verticies:
        [1 | 0]
        [2 | 3]
        For triangulation */
    const FIntVector8 triangulationTable[3][4] = {
        {{0,0,0}, {0, -1, 0}, {0, -1, -1}, {0, 0, -1}}, // X
        {{0,0,0}, {0, 0, -1}, {-1, 0,-1}, {-1, 0, 0} }, // Y
        {{0,0,0}, {-1, 0, 0}, { -1, -1, 0}, {0, -1, 0}} // Z
    };

    const uint8 Z_CHUNK_COUNT = 4;
    const uint8 Z_CHUNK_BOUNDS = Z_CHUNK_COUNT / 2;
    const uint8 CHUNK_COUNT = 32;
    const uint8 CHUNK_BOUNDS = CHUNK_COUNT / 2;

    const uint16 CHUNK_SIZE = 4096;
    const int WORLD_SIZE = CHUNK_SIZE * CHUNK_BOUNDS;

    const uint8 MAX_CHUNK_RESOLUTION = 32;
    const uint16 VOXEL_SIZE = CHUNK_SIZE / MAX_CHUNK_RESOLUTION;
    const uint8 DISTANCE_FIELD_SIZE = MAX_CHUNK_RESOLUTION + 1;
    const uint8 MAX_LOD_DEPTH = FMath::Log2((uint8)MAX_CHUNK_RESOLUTION);

    static FVector ChunkCoordinateToWorld(int ChunkX, int ChunkY, int ChunkZ, int X, int Y, int Z) {
        return FVector(ChunkX * CHUNK_SIZE + X * VOXEL_SIZE, ChunkY * CHUNK_SIZE + Y * VOXEL_SIZE, ChunkZ * CHUNK_SIZE + Z * VOXEL_SIZE);
    }

    static FVector2D ChunkCoordinateToWorld2D(int ChunkX, int ChunkY, int X, int Y) {
        return FVector2D(ChunkX * CHUNK_SIZE + X * VOXEL_SIZE, ChunkY * CHUNK_SIZE + Y * VOXEL_SIZE);
    }

    static int16 FloatDistanceToCompressed(float Distance) {
        return (FMath::Clamp(Distance, -(float)CHUNK_SIZE, (float)CHUNK_SIZE) / CHUNK_SIZE) * INT16_MAX;
    }

    static float CompressedDistanceToFloat(int16 CompressedDistance) {
        return ((float)CompressedDistance / INT16_MAX) * CHUNK_SIZE;
    }
}