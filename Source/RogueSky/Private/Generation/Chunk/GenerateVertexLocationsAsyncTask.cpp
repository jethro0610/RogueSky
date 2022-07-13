#include "Generation/Chunk/GenerateVertexLocationsAsyncTask.h"
#include "Generation/DataTypes/GenerationConstants.h"
#include "Generation/Chunk/Chunk.h"
using namespace GenerationConstants;

GenerateVertexLocationsAsyncTask::GenerateVertexLocationsAsyncTask(Chunk* Chunk) :
    chunk(Chunk),
    chunkLocation(Chunk->GetLocation())
{ }

void GenerateVertexLocationsAsyncTask::DoWork() {
    for (int x = 0; x < MAX_CHUNK_RESOLUTION; x++)
    for (int y = 0; y < MAX_CHUNK_RESOLUTION; y++)
    for (int z = 0; z < MAX_CHUNK_RESOLUTION; z++) {
        FIntVector8 location(x, y, z);
        // Find the vector location of the vertex
        FVector intersectionSum = FVector::ZeroVector;
        int totalIntersections = 0;
        for (int e = 0; e < 12; e++) {
            FIntVector8 e1Loc = location + cornerTable[edgeTable[e][0]];
            FIntVector8 e2Loc = location + cornerTable[edgeTable[e][1]];
            FFloat16 e1 = chunk->distanceField.GetDistance(e1Loc);
            FFloat16 e2 = chunk->distanceField.GetDistance(e2Loc);

            // If there was a sign change along the edge then create a vertex on that edge
            if (e1 * e2 <= 0.0f) {
                float differenceRatio = (float)e1 / (e1 - e2);
                FVector intersection = FVector::ZeroVector;

                // Ensure there's no zero division and calculate the intersection point
                if (e1 - e2 == 0.0f)
                    intersection = (e1Loc.ToVector() + e2Loc.ToVector()) / 2.0f;
                else
                    intersection = (1.0f - differenceRatio) * e1Loc.ToVector() + differenceRatio * e2Loc.ToVector();

                intersectionSum += intersection;
                totalIntersections++;
            }
        }
        if (totalIntersections != 0) { // Find the average of all the points on the edges and set the vertex location
            FVector vertexLocation = ((chunkLocation.ToVector() * MAX_CHUNK_RESOLUTION) + (intersectionSum / totalIntersections)) * (CHUNK_SIZE / MAX_CHUNK_RESOLUTION);
            chunk->SetVertexLocation(x, y, z, vertexLocation);
        }
    }
    chunk->SetState(ChunkState::HasOctree);
}