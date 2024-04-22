#pragma once

#include "Vector4.h"
#include <stdexcept>

struct Matrix4
{
	float m[4][4];

	// Default constructor delegating to the Identity initialization
	Matrix4() : Matrix4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f)
	{}

	// Constructor to initialize the matrix elements
	Matrix4(
		float i_11, float i_12, float i_13, float i_14,
		float i_21, float i_22, float i_23, float i_24,
		float i_31, float i_32, float i_33, float i_34,
		float i_41, float i_42, float i_43, float i_44) {
		m[0][0] = i_11; m[0][1] = i_12; m[0][2] = i_13; m[0][3] = i_14;
		m[1][0] = i_21; m[1][1] = i_22; m[1][2] = i_23; m[1][3] = i_24;
		m[2][0] = i_31; m[2][1] = i_32; m[2][2] = i_33; m[2][3] = i_34;
		m[3][0] = i_41; m[3][1] = i_42; m[3][2] = i_43; m[3][3] = i_44;
	}

	// Copy constructor
	Matrix4(const Matrix4& i_other) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m[i][j] = i_other.m[i][j];
			}
		}
	}

	// Assignment operator
	Matrix4& operator=(const Matrix4& i_other) {
		if (this != &i_other) {
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					m[i][j] = i_other.m[i][j];
				}
			}
		}
		return *this;
	}

	// matrix multiplication
	Matrix4 operator*(const Matrix4& i_other) const {
		Matrix4 result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = 0;
				for (int k = 0; k < 4; ++k) {
					result.m[i][j] += m[i][k] * i_other.m[k][j];
				}
			}
		}
		return result;
	}

	// Multiply matrix by vector (as column vector)
	Vector4 operator*(const Vector4& vec) const {
		return Vector4(
			m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3] * vec.w,
			m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3] * vec.w,
			m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3] * vec.w,
			m[3][0] * vec.x + m[3][1] * vec.y + m[3][2] * vec.z + m[3][3] * vec.w
		);
	}

	Vector3 TransformNormalRV(const Vector3& i_Normal) const {
		Vector4 v(i_Normal, 0);
		Vector4 transformed = v* (*this);
		return Vector3(transformed.x, transformed.y, transformed.z);
		// calculate this * v;
	};
	Vector3 TransformNormalCV(const Vector3& i_Normal) const {
		Vector4 v(i_Normal, 0);
		Vector4 transformed = *this * v;
		return Vector3(transformed.x, transformed.y, transformed.z);
	};

	// Grab a particular row or column
	Vector4 Row(size_t i_row) const {
		if (i_row >= 0 && i_row < 4) {
			return Vector4(m[i_row][0], m[i_row][1], m[i_row][2], m[i_row][3]);
		}
		else {
			return Vector4::Zero;
		}
	};

	Vector4 Col(size_t i_col) const {
		if (i_col >= 0 && i_col < 4) {
			return Vector4(m[0][i_col], m[1][i_col], m[2][i_col], m[3][i_col]);
		}
		else {
			return Vector4::Zero;
		}
	};

	// Useful matrix creators
	static Matrix4 CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ) {
		return Matrix4(
			i_scaleX, 0.0f, 0.0f, 0.0f,
			0.0f, i_scaleY, 0.0f, 0.0f,
			0.0f, 0.0f, i_scaleZ, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};

	/* 
	* I can't think of why row vector needs these
	* I won't implement these unless they are used
	* 
	// Layout for row vector multiplies ( V = V * M )
	static Matrix4 CreateXRotationRV(float i_RotationRadians);
	static Matrix4 CreateYRotationRV(float i_RotationRadians);
	static Matrix4 CreateZRotationRV(float i_RotationRadians);

	static Matrix4 CreateTranslationRV(float i_transX, float i_transY, float i_transZ);
	inline static Matrix4 CreateTranslationRV(const Vector3& i_trans);
	*/

	// Layout for column vector multiplies ( V = M * V )

	// rotation matrix
	static Matrix4 CreateXRotationCV(float i_RotationRadians) {
		float c = cos(i_RotationRadians);
		float s = sin(i_RotationRadians);
		return Matrix4(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1
		);
	};
	static Matrix4 CreateYRotationCV(float i_RotationRadians) {
		float c = cos(i_RotationRadians);
		float s = sin(i_RotationRadians);
		return Matrix4(
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1
		);
	};
	static Matrix4 CreateZRotationCV(float i_RotationRadians) {
		float c = cos(i_RotationRadians);
		float s = sin(i_RotationRadians);
		return Matrix4(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};

	// translation matrix
	static Matrix4 CreateTranslationCV(float i_transX, float i_transY, float i_transZ) {
		return Matrix4(
			1.0f, 0.0f, 0.0f, i_transX,
			0.0f, 1.0f, 0.0f, i_transY,
			0.0f, 0.0f, 1.0f, i_transZ,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	inline static Matrix4 CreateTranslationCV(const Vector3& i_trans) {
		return CreateTranslationCV(i_trans.x, i_trans.y, i_trans.z);
	};


	// Helper function to calculate the determinant of a 3x3 matrix
	float Det3x3(int r1, int r2, int r3, int c1, int c2, int c3) const {
		return m[r1][c1] * (m[r2][c2] * m[r3][c3] - m[r2][c3] * m[r3][c2])
			- m[r1][c2] * (m[r2][c1] * m[r3][c3] - m[r2][c3] * m[r3][c1])
			+ m[r1][c3] * (m[r2][c1] * m[r3][c2] - m[r2][c2] * m[r3][c1]);
	}

	// Calculate the determinant of this 4x4 matrix
	float Determinant() const {
		return m[0][0] * Det3x3(1, 2, 3, 1, 2, 3)
			- m[0][1] * Det3x3(1, 2, 3, 0, 2, 3)
			+ m[0][2] * Det3x3(1, 2, 3, 0, 1, 3)
			- m[0][3] * Det3x3(1, 2, 3, 0, 1, 2);
	}

	// Compute the adjugate of the matrix
	Matrix4 Adjugate() const {
		Matrix4 adj;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				// Compute the minor of m[i][j]
				int sign = (i + j) % 2 == 0 ? 1 : -1;
				adj.m[j][i] = sign * Det3x3((i + 1) % 4, (i + 2) % 4, (i + 3) % 4,
					(j + 1) % 4, (j + 2) % 4, (j + 3) % 4);  // Note the transpose for adjugate
			}
		}
		return adj;
	}

	// Invert this matrix in place
	void Invert() {
		float det = Determinant();
		if (det == 0) {
			throw std::runtime_error("Matrix is singular and cannot be inverted.");
		}

		Matrix4 adj = Adjugate();
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m[i][j] = adj.m[i][j] / det;
			}
		}
	}

	// Return the inverse of this matrix without modifying this instance
	Matrix4 GetInverse() const {
		Matrix4 copy = *this;
		copy.Invert();
		return copy;
	}

	// Store the inverse of this matrix in an output parameter
	void GetInverse(Matrix4& o_out) const {
		o_out = GetInverse();
	}



	// Return a matrix that is the inverse ASSUMING this matrix has axis rotation and translation only.
	Matrix4 GetInverseRotTransCV() const {
		Matrix4 inv;
		// Transpose the 3x3 rotation part
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				inv.m[j][i] = m[i][j];  // Transpose rotation part
			}
		}
		// Calculate the negative translated position using the transposed rotation part
		inv.m[0][3] = -(m[0][0] * m[0][3] + m[1][0] * m[1][3] + m[2][0] * m[2][3]);
		inv.m[1][3] = -(m[0][1] * m[0][3] + m[1][1] * m[1][3] + m[2][1] * m[2][3]);
		inv.m[2][3] = -(m[0][2] * m[0][3] + m[1][2] * m[1][3] + m[2][2] * m[2][3]);

		// Last row remains the same for homogeneous coordinates in affine transformations
		inv.m[3][0] = 0.0f;
		inv.m[3][1] = 0.0f;
		inv.m[3][2] = 0.0f;
		inv.m[3][3] = 1.0f;

		return inv;
	}

	/*
	* ignore the row vector for now
	Matrix4 GetInverseRotTransRV() const;
	*/

	// Transpose this instance in place
	void Transpose() {
		for (int i = 0; i < 4; i++) {
			for (int j = i + 1; j < 4; j++) {
				std::swap(m[i][j], m[j][i]);
			}
		}
	}

	// Return a matrix that is a transpose of this instance but don't modify this instance
	Matrix4 GetTranspose() const {
		Matrix4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = m[j][i];  // Swap rows and columns
			}
		}
		return result;
	}

	// Store the transpose of this matrix in an output parameter
	void GetTranspose(Matrix4& o_out) const {
		o_out = GetTranspose();
	}

	// Multiply Matrix * Matrix
	// return *this * i_other
	void Multiply(const Matrix4& i_other, Matrix4& o_out) const {
		o_out = *this * i_other;
	};

	// Multiply Vector by Matrix
	// return v * M
	Vector4 MultiplyLeft(const Vector4& i_Vector) const {
		return i_Vector * (*this);

	};
	// return M * v
	Vector4 MultiplyRight(const Vector4& i_Vector) const {
		return *this * i_Vector;
	};

	static const Matrix4 Identity;
};

const Matrix4 Matrix4::Identity(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 1.0f, 0.0f, 
	0.0f, 0.0f, 0.0f, 1.0f);
