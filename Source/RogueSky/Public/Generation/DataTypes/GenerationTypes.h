// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GenerationTypes.generated.h"

/**
 * 
 */
USTRUCT()
struct FIntVector8 {
	GENERATED_BODY()

	int8 X;
	int8 Y;
	int8 Z;

	FIntVector8() {
		X = 0;
		Y = 0;
		Z = 0;
	}

	FIntVector8(int8 X, int8 Y) {
		this->X = X;
		this->Y = Y;
	}

	FIntVector8(int8 X, int8 Y, int8 Z) {
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	FIntVector8(FVector Vector) {
		X = Vector.X;
		Y = Vector.Y;
		Z = Vector.Z;
	}

	bool operator==(const FIntVector8& IntVector) const {
		return (X == IntVector.X && Y == IntVector.Y && Z == IntVector.Z);
	}

	bool operator!=(const FIntVector8& IntVector) const {
		return (X != IntVector.X || Y != IntVector.Y || Z != IntVector.Z);
	}

	FIntVector8 operator+(const FIntVector8& IntVector) const{
		return FIntVector8(X + IntVector.X, Y + IntVector.Y, Z + IntVector.Z);
	}

	FIntVector8& operator+=(const FIntVector8& IntVector) {
		X += IntVector.X;
		Y += IntVector.Y;
		Z += IntVector.Z;

		return *this;
	}

	FIntVector8 operator-(const FIntVector8& IntVector) const{
		return FIntVector8(X - IntVector.X, Y - IntVector.Y, Z - IntVector.Z);
	}

	FIntVector8& operator-=(const FIntVector8& IntVector) {
		X -= IntVector.X;
		Y -= IntVector.Y;
		Z -= IntVector.Z;

		return *this;
	}

	FIntVector8 operator*(const int Scalar) const{
		return FIntVector8(X * Scalar, Y * Scalar, Z * Scalar);
	}

	friend uint32 GetTypeHash(const FIntVector8& IntVector) {
		uint32 Hash = FCrc::MemCrc32(&IntVector, sizeof(FIntVector8));
		return Hash;
	}

public:
	FVector ToVector() const {
		return FVector(X, Y, Z);
	}

	FString ToString() const {
		return "X: " + FString::FromInt(X) + " Y: " + FString::FromInt(Y) + " Z: " + FString::FromInt(Z);
	}
};

USTRUCT()
struct FIntPoint8 {
	GENERATED_BODY()

	int8 X;
	int8 Y;

	FIntPoint8() {
		X = 0;
		Y = 0;
	}

	FIntPoint8(int X, int Y) {
		this->X = X;
		this->Y = Y;
	}

	FIntPoint8(FVector2D Vector) {
		X = Vector.X;
		Y = Vector.Y;
	}

	bool operator==(const FIntPoint8& IntPoint) const {
		return (X == IntPoint.X && Y == IntPoint.Y);
	}

	bool operator!=(const FIntPoint8& IntPoint) const {
		return (X != IntPoint.X || Y != IntPoint.Y);
	}

	FIntPoint8 operator+(const FIntPoint8& IntPoint) const {
		return FIntPoint8(X + IntPoint.X, Y + IntPoint.Y);
	}

	FIntPoint8& operator+=(const FIntPoint8& IntPoint) {
		X += IntPoint.X;
		Y += IntPoint.Y;

		return *this;
	}

	FIntPoint8 operator-(const FIntPoint8& IntPoint) const {
		return FIntPoint8(X - IntPoint.X, Y - IntPoint.Y);
	}

	FIntPoint8& operator-=(const FIntPoint8& IntPoint) {
		X -= IntPoint.X;
		Y -= IntPoint.Y;

		return *this;
	}

	FIntPoint8 operator*(const int Scalar) const {
		return FIntPoint8(X * Scalar, Y * Scalar);
	}

	friend uint32 GetTypeHash(const FIntPoint8& IntPoint) {
		uint32 Hash = FCrc::MemCrc32(&IntPoint, sizeof(FIntPoint8));
		return Hash;
	}

public:
	FVector2D ToVector() const {
		return FVector2D(X, Y);
	}

	FString ToString() const {
		return "X: " + FString::FromInt(X) + " Y: " + FString::FromInt(Y);
	}
};