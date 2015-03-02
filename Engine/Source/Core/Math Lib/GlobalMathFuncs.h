#ifndef GLOBALMATHFUNCS_H
#define GLOBALMATHFUNCS_H

//#ifndef GLOBALMATHFUNCS_INL
//#define GLOBALMATHFUNCS_INL

#include "MathDeclarations.h"

#ifndef MATHLIB_H
#error "Don't Include GlobalMathFunc.h Directly. Include MathLib.h Instead."
#endif // MATHLIB_H


// Utility Functions

template<typename Type>
Type Max(Type lhs, Type rhs);

template<typename Type>
Type Min(Type lhs, Type rhs);

template<typename Type>
Type Clamp(Type val, Type minVal, Type maxVal);

template<typename Type>
Type Saturate(Type val);

float Lerp(const float& lhs, const float& rhs, const float& fLambda);
double Lerp(const double& lhs, const double& rhs, const double& fLambda);
Vec2f_Out Lerp(Vec2f_In lhs, Vec2f_In rhs, const float& fLambda);
Vec3f_Out Lerp(Vec3f_In lhs, Vec3f_In rhs, const float& fLambda);
Vec4f_Out Lerp(Vec4f_In lhs, Vec4f_In rhs, const float& fLambda);

template<int pX, int pY> Vec2f_Out Permute(Vec2f_In lhs);
template<int pX, int pY, int pZ> Vec3f_Out Permute(Vec3f_In lhs);
template<int pX, int pY, int pZ, int pW> Vec4f_Out Permute(Vec4f_In lhs);

template<int pX, int pY> Vec2f_Out Permute(Vec2f_In lhs, Vec2f_In rhs);
template<int pX, int pY, int pZ> Vec3f_Out Permute(Vec3f_In lhs, Vec3f_In rhs);
template<int pX, int pY, int pZ, int pW> Vec4f_Out Permute(Vec4f_In lhs, Vec4f_In rhs);

// Vector Math Functions

float Dot(Vec2f_In vVectorA, Vec2f_In vVectorB);
float Dot(Vec3f_In vVectorA, Vec3f_In vVectorB);
float Dot(Vec4f_In vVectorA, Vec4f_In vVectorB);

float Mag(Vec2f_In vVector);
float Mag(Vec3f_In vVector);
float Mag(Vec4f_In vVector);

float Length(Vec2f_In vVector);
float Length(Vec3f_In vVector);
float Length(Vec4f_In vVector);

float MagSq(Vec2f_In vVector);
float MagSq(Vec3f_In vVector);
float MagSq(Vec4f_In vVector);

float LengthSq(Vec2f_In vVector);
float LengthSq(Vec3f_In vVector);
float LengthSq(Vec4f_In vVector);

Vec2f_Out Normalize(Vec2f_In vVector);
Vec3f_Out Normalize(Vec3f_In vVector);
Vec4f_Out Normalize(Vec4f_In vVector);
Mat33f_Out Normalize(Mat33f_In mMatrix);
Mat44f_Out Normalize(Mat44f_In mMatrix);

Vec2f_Out IntToFloat(Vec2f_In vVector);
Vec3f_Out IntToFloat(Vec3f_In vVector);
Vec4f_Out IntToFloat(Vec4f_In vVector);

Vec2f_Out FloatToInt(Vec2f_In vVector);
Vec3f_Out FloatToInt(Vec3f_In vVector);
Vec4f_Out FloatToInt(Vec4f_In vVector);

Vec2f_Out Floor(Vec2f_In vVector);
Vec3f_Out Floor(Vec3f_In vVector);
Vec4f_Out Floor(Vec4f_In vVector);

Vec2f_Out Ceil(Vec2f_In vVector);
Vec3f_Out Ceil(Vec3f_In vVector);
Vec4f_Out Ceil(Vec4f_In vVector);

Vec2f_Out Trunc(Vec2f_In vVector);
Vec3f_Out Trunc(Vec3f_In vVector);
Vec4f_Out Trunc(Vec4f_In vVector);

Vec2f_Out Round(Vec2f& vVector);
Vec3f_Out Round(Vec3f_In vVector);
Vec4f_Out Round(Vec4f_In vVector);

Mat44f_Out Lerp(Mat44f_In MatrixA, Mat44f_In MatrixB, const float fLambda);

Vec3f_Out Cross(Vec3f_In lhs, Vec3f_In rhs);


// Matrix Function

Mat44f_Out MatrixInverse(Mat44f_In mMatrix);

Mat33f_Out Transpose(Mat33f_In mMatrix);
Mat44f_Out Transpose(Mat44f_In mMatrix);

Mat44f_Out Transpose3x3(Mat44f_In mMatrix);

Mat44f_Out RotateAround(Vec3f_In origin, Mat44f_In rotatorsMatrix, Mat44f_In rotationMatrix);

Mat44f_Out MakePerspectiveMatrix(float fFOV, float fAspectRatio, float fNearClip, float fFarClip);
Mat44f_Out MakeOrthographicMatrix(float fWidth, float fHeight, float fNear, float fFar);
Mat44f_Out MakeTextureMatrixOffset(unsigned int unWidth, unsigned int unHeight);

float CalculateGaussianWeight(int nOffset, float fSigma = 1.0f);

// unRadius - Number of Pixels to Blur In a Single Direction Including the Center Pixel
inline void CalculateGaussianWeights(_Out_writes_all_(unRadius) float* pGaussianWeights, unsigned int unRadius, float fLimit = 1.0f);

#if SSE_AVAILABLE
ScalarV_Out Lerp(ScalarV_In lhs, ScalarV_In rhs, ScalarV_In vLambda);
Vec2V_Out Lerp(Vec2V_In lhs, Vec2V_In rhs, ScalarV_In vLambda);
Vec3V_Out Lerp(Vec3V_In lhs, Vec3V_In rhs, ScalarV_In vLambda);
Vec4V_Out Lerp(Vec4V_In lhs, Vec4V_In rhs, ScalarV_In vLambda);

template<int pX, int pY> Vec2V_Out Permute(Vec2V_In lhs);
template<int pX, int pY, int pZ> Vec3V_Out Permute(Vec3V_In lhs);
template<int pX, int pY, int pZ, int pW> Vec4V_Out Permute(Vec4V_In lhs);

template<int pX, int pY> Vec2V_Out Permute(Vec2V_In lhs, Vec2V_In rhs);
template<int pX, int pY, int pZ> Vec3V_Out Permute(Vec3V_In lhs, Vec3V_In rhs);
template<int pX, int pY, int pZ, int pW> Vec4V_Out Permute(Vec4V_In lhs, Vec4V_In rhs);

template<int elem> ScalarV_Out ScalarVFromElement(Vector_In vVector);
template<int elem> ScalarV_Out ScalarVFromElement(Vec2V_In vVector);
template<int elem> ScalarV_Out ScalarVFromElement(Vec3V_In vVector);
template<int elem> ScalarV_Out ScalarVFromElement(Vec4V_In vVector);

// Vector Math Functions

ScalarV_Out Sqrt(ScalarV_In vScalar);

ScalarV Dot(Vec2V_In vVectorA, Vec2V_In vVectorB);
ScalarV Dot(Vec3V_In vVectorA, Vec3V_In vVectorB);
ScalarV Dot(Vec4V_In vVectorA, Vec4V_In vVectorB);

ScalarV Mag(Vec2V_In vVector);
ScalarV Mag(Vec3V_In vVector);
ScalarV Mag(Vec4V_In vVector);

ScalarV Length(Vec2V_In vVector);
ScalarV Length(Vec3V_In vVector);
ScalarV Length(Vec4V_In vVector);

ScalarV MagSq(Vec2V_In vVector);
ScalarV MagSq(Vec3V_In vVector);
ScalarV MagSq(Vec4V_In vVector);

ScalarV LengthSq(Vec2V_In vVector);
ScalarV LengthSq(Vec3V_In vVector);
ScalarV LengthSq(Vec4V_In vVector);

Vec2V_Out Normalize(Vec2V_In vVector);
Vec3V_Out Normalize(Vec3V_In vVector);
Vec4V_Out Normalize(Vec4V_In vVector);
//Mat33V_Out Normalize(Mat33V_In mMatrix);
Mat44V_Out Normalize(Mat44V_In mMatrix);

Vec2V_Out AddInt(Vec2V_In lhs, Vec2V_In rhs);
Vec3V_Out AddInt(Vec3V_In lhs, Vec3V_In rhs);
Vec4V_Out AddInt(Vec4V_In lhs, Vec4V_In rhs);

Vec2V_Out SubtractInt(Vec2V_In lhs, Vec2V_In rhs);
Vec3V_Out SubtractInt(Vec3V_In lhs, Vec3V_In rhs);
Vec4V_Out SubtractInt(Vec4V_In lhs, Vec4V_In rhs);

Vec2V_Out MultiplyInt(Vec2V_In lhs, Vec2V_In rhs);
Vec3V_Out MultiplyInt(Vec3V_In lhs, Vec3V_In rhs);
Vec4V_Out MultiplyInt(Vec4V_In lhs, Vec4V_In rhs);

Vec2V_Out IntToFloat(Vec2V_In vVector);
Vec3V_Out IntToFloat(Vec3V_In vVector);
Vec4V_Out IntToFloat(Vec4V_In vVector);

Vec2V_Out FloatToInt(Vec2V_In vVector);
Vec3V_Out FloatToInt(Vec3V_In vVector);
Vec4V_Out FloatToInt(Vec4V_In vVector);

Vec2V_Out Floor(Vec2V_In vVector);
Vec3V_Out Floor(Vec3V_In vVector);
Vec4V_Out Floor(Vec4V_In vVector);

Vec2V_Out Ceil(Vec2V_In vVector);
Vec3V_Out Ceil(Vec3V_In vVector);
Vec4V_Out Ceil(Vec4V_In vVector);

Vec2V_Out Trunc(Vec2V_In vVector);
Vec3V_Out Trunc(Vec3V_In vVector);
Vec4V_Out Trunc(Vec4V_In vVector);

Vec2V_Out Round(Vec2V_In vVector);
Vec3V_Out Round(Vec3V_In vVector);
Vec4V_Out Round(Vec4V_In vVector);

Mat44V_Out Lerp(Mat44V_In MatrixA, Mat44V_In MatrixB, ScalarV_In vLambda);

Vec3V_Out CrossProduct(Vec3V_In lhs, Vec3V_In rhs);

Mat44V_Out MatrixInverse(Mat44V_In mMatrix);

Mat44V_Out Transpose(Mat44V_In mMatrix);

Mat44V_Out Transpose3x3(Mat44V_In mMatrix);

Mat44V_Out RotateAround(Vec3V_In origin, Mat44V_In rotatorsMatrix, Mat44V_In rotationMatrix);

Mat44V MakePerspectiveMatrixV(float fFOV, float fAspectRatio, float fNearClip, float fFarClip);

Mat44V MakeOrthographicMatrixV(float fWidth, float fHeight, float fNear, float fFar);

Mat44V MakeTextureMatrixOffsetV(unsigned int unWidth, unsigned int unHeight);
#endif //SSE_AVAILABLE

//#endif //GLOBALMATHFUNCS_INL

#include "GlobalMathFuncs.inl"

#endif // GLOBALMATHFUNCS_H