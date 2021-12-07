#ifndef matrix_defined
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define matrix_defined

struct Mat {
	float* entries;
	int row;
	int col;
};

void showmat(struct Mat* A);
struct Mat* newmat(int r, int c, float d);
void freemat(struct Mat* A);
float innermultiply(struct Mat* a, struct Mat* b);
struct Mat* copyvalue(struct Mat* A);
struct Mat* inverse(struct Mat* A);
struct Mat* adjoint(struct Mat* A);
float trace(struct Mat* A);
struct Mat* multiply(struct Mat* A, struct Mat* B);
struct Mat* scalermultiply(struct Mat* M, float c);
struct Mat* eye(int n);

void showmat(struct Mat* A) {
	if (A->row > 0 && A->col > 0) {
		int k = 0;
		printf("[");
		for (int i = 1;i <= A->row;i++) {
			for (int j = 1;j <= A->col;j++) {
				if (j < A->col) {
					printf("%f\t", A->entries[k++]);
				} else {
					printf("%f", A->entries[k++]);
				}
			}
			if (i < A->row) {
				printf("\n");
			} else {
				printf("]\n");
			}
		}
		printf("\n");
	} else {
		printf("[]");
	}
}

struct Mat* newmat(int r, int c, float d) {
	struct Mat* M = (struct Mat*) malloc(sizeof(struct Mat));
	M->row = r;M->col = c;
	M->entries = (float*) malloc(sizeof(float) * r * c);
	int k = 0;
	for (int i = 1;i <= M->row;i++) {
		for (int j = 1;j <= M->col;j++) {
			M->entries[k++] = d;
		}
	}
	return M;
}

void freemat(struct Mat* A) {
	free(A->entries);
	free(A);
}

struct Mat* eye(int n) {
	struct Mat* I = newmat(n, n, 0);
	for (int i = 1;i <= n;i++) {
		I->entries[(i - 1) * n + i - 1] = 1;
	}
	return I;
}
struct Mat* zeros(int r, int c) {
	struct Mat* Z = newmat(r, c, 0);
	return Z;
}
struct Mat* ones(int r, int c) {
	struct Mat* O = newmat(r, c, 1);
	return O;
}
struct Mat* randm(int r, int c, float l, float u) {
	struct Mat* R = newmat(r, c, 1);
	int k = 0;
	for (int i = 1;i <= r;i++) {
		for (int j = 1;j <= c;j++) {
			float r = ((float) rand()) / ((float) RAND_MAX);
			R->entries[k++] = l + (u - l) * r;
		}
	}
	return R;
}
float mat_get(struct Mat* M, int r, int c) {
	return M->entries[(r - 1) * M->col + c - 1];
}
void mat_set(struct Mat* M, int r, int c, float d) {
	M->entries[(r - 1) * M->col + c - 1] = d;
}

struct Mat* scalermultiply(struct Mat* M, float c) {
	struct Mat* B = newmat(M->row, M->col, 0);
	int k = 0;
	for (int i = 0;i < M->row;i++) {
		for (int j = 0;j < M->col;j++) {
			B->entries[k] = M->entries[k] * c;
			k += 1;
		}
	}
	return B;
}
struct Mat* sum(struct Mat* A, struct Mat* B) {
	int r = A->row;
	int c = A->col;
	struct Mat* C = newmat(r, c, 0);
	int k = 0;
	for (int i = 0;i < r;i++) {
		for (int j = 0;j < c;j++) {
			C->entries[k] = A->entries[k] + B->entries[k];
			k += 1;
		}
	}
	return C;
}
struct Mat* minus(struct Mat* A, struct Mat* B) {
	int r = A->row;
	int c = A->col;
	struct Mat* C = newmat(r, c, 0);
	int k = 0;
	for (int i = 0;i < r;i++) {
		for (int j = 0;j < c;j++) {
			C->entries[k] = A->entries[k] - B->entries[k];
			k += 1;
		}
	}
	return C;
}
struct Mat* submat(struct Mat* A, int r1, int r2, int c1, int c2) {
	struct Mat* B = newmat(r2 - r1 + 1, c2 - c1 + 1, 0);
	int k = 0;
	for (int i = r1;i <= r2;i++) {
		for (int j = c1;j <= c2;j++) {
			B->entries[k++] = A->entries[(i - 1) * A->col + j - 1];
		}
	}
	return B;
}
void submat2(struct Mat* A, struct Mat* B, int r1, int r2, int c1, int c2) {
	int k = 0;
	for (int i = r1;i <= r2;i++) {
		for (int j = c1;j <= c2;j++) {
			B->entries[k++] = A->entries[(i - 1) * A->col + j - 1];
		}
	}
}

struct Mat* multiply(struct Mat* A, struct Mat* B) {
	int r1 = A->row;
	int r2 = B->row;
	int c1 = A->col;
	int c2 = B->col;
	if (r1 == 1 && c1 == 1) {
		struct Mat* C = scalermultiply(B, A->entries[0]);
		return C;
	} else if (r2 == 1 && c2 == 1) {
		struct Mat* C = scalermultiply(A, B->entries[0]);
		return C;
	}
	struct Mat* C = newmat(r1, c2, 0);
	for (int i = 1;i <= r1;i++) {
		for (int j = 1;j <= c2;j++) {
			float de = 0;
			for (int k = 1;k <= r2;k++) {
				de += A->entries[(i - 1) * A->col + k - 1] * B->entries[(k - 1) * B->col + j - 1];
			}
			C->entries[(i - 1) * C->col + j - 1] = de;
		}
	}
	return C;
}
struct Mat* removerow(struct Mat* A, int r) {
	struct Mat* B = newmat(A->row - 1, A->col, 0);
	int k = 0;
	for (int i = 1;i <= A->row;i++) {
		for (int j = 1;j <= A->col;j++) {
			if (i != r) {
				B->entries[k] = A->entries[(i - 1) * A->col + j - 1];
				k += 1;
			}
		}
	}
	return B;
}
struct Mat* removecol(struct Mat* A, int c) {
	struct Mat* B = newmat(A->row, A->col - 1, 0);
	int k = 0;
	for (int i = 1;i <= A->row;i++) {
		for (int j = 1;j <= A->col;j++) {
			if (j != c) {
				B->entries[k] = A->entries[(i - 1) * A->col + j - 1];
				k += 1;
			}
		}
	}
	return B;
}
void removerow2(struct Mat* A, struct Mat* B, int r) {
	int k = 0;
	for (int i = 1;i <= A->row;i++) {
		for (int j = 1;j <= A->col;j++) {
			if (i != r) {
				B->entries[k++] = A->entries[(i - 1) * A->col + j - 1];
			}
		}
	}
}
void removecol2(struct Mat* A, struct Mat* B, int c) {
	int k = 0;
	for (int i = 1;i <= A->row;i++) {
		for (int j = 1;j <= A->col;j++) {
			if (j != c) {
				B->entries[k++] = A->entries[(i - 1) * A->col + j - 1];
			}
		}
	}
}
struct Mat* transpose(struct Mat* A) {
	struct Mat* B = newmat(A->col, A->row, 0);
	int k = 0;
	for (int i = 1;i <= A->col;i++) {
		for (int j = 1;j <= A->row;j++) {
			B->entries[k] = A->entries[(j - 1) * A->row + i - 1];
			k += 1;
		}
	}
	return B;
}
float det(struct Mat* M) {
	int r = M->row;
	int c = M->col;
	if (r == 1 && c == 1) {
		float d = M->entries[0];
		return d;
	}
	struct Mat* M1 = removerow(M, 1);
	struct Mat* M2 = newmat(M->row - 1, M->col - 1, 0);
	float d = 0, si = +1;
	for (int j = 1;j <= M->col;j++) {
		float c = M->entries[j - 1];
		removecol2(M1, M2, j);
		d += si * det(M2) * c;
		si *= -1;
	}
	freemat(M1);
	freemat(M2);
	return d;
}

float trace(struct Mat* A) {
	float d = 0;
	for (int i = 1;i <= A->row;i++) {
		d += A->entries[(i - 1) * A->row + i - 1];
	}
	return d;
}

struct Mat* adjoint(struct Mat* A) {
	struct Mat* B = newmat(A->row, A->col, 0);
	struct Mat* A1 = newmat(A->row - 1, A->col, 0);
	struct Mat* A2 = newmat(A->row - 1, A->col - 1, 0);
	for (int i = 1;i <= A->row;i++) {
		removerow2(A, A1, i);
		for (int j = 1;j <= A->col;j++) {
			removecol2(A1, A2, j);
			float si = pow(-1, (float) (i + j));
			B->entries[(i - 1) * B->col + j - 1] = det(A2) * si;
		}
	}
	struct Mat* C = transpose(B);
	freemat(A1);
	freemat(A2);
	freemat(B);
	return C;
}

struct Mat* inverse(struct Mat* A) {
	struct Mat* B = adjoint(A);
	float de = det(A);
	struct Mat* C = scalermultiply(B, 1 / de);
	freemat(B);
	return C;
}

struct Mat* copyvalue(struct Mat* A) {
	struct Mat* B = newmat(A->row, A->col, 0);
	int k = 0;
	for (int i = 1;i <= A->row;i++) {
		for (int j = 1;j <= A->col;j++) {
			B->entries[k] = A->entries[k];
			k++;
		}
	}
	return B;
}
struct Mat* triinverse(struct Mat* A) {
	struct Mat* B = newmat(A->row, A->col, 0);
	for (int i = 1;i <= B->row;i++) {
		for (int j = i;j <= B->col;j++) {
			if (i == j) {
				B->entries[(i - 1) * B->col + j - 1] = 1 / A->entries[(i - 1) * A->col + j - 1];
			} else {
				B->entries[(i - 1) * B->col + j - 1] = -A->entries[(i - 1) * A->col + j - 1] / A->entries[(j - 1) * A->col + j - 1];
			}
		}
	}
	return B;
}
struct Mat* rowechelon(struct Mat* A) {
	if (A->row == 1) {
		for (int j = 1;j <= A->col;j++) {
			if (A->entries[j - 1] != 0) {
				struct Mat* B = scalermultiply(A, 1 / A->entries[j - 1]);
				return B;
			}
		}
		struct Mat* B = newmat(1, A->col, 0);
		return B;
	}
	struct Mat* B = copyvalue(A);
	int ind1 = B->col + 1;
	int ind2 = 1;
	for (int i = 1;i <= B->row;i++) {
		for (int j = 1;j <= B->col;j++) {
			if (B->entries[(i - 1) * B->col + j - 1] != 0 && j < ind1) {
				ind1 = j;
				ind2 = i;
				break;
			}
		}
	}
	if (ind2 > 1) {
		for (int j = 1;j <= B->col;j++) {
			float temp = B->entries[j - 1];
			B->entries[j - 1] = B->entries[(ind2 - 1) * B->col + j - 1];
			B->entries[(ind2 - 1) * B->col + j - 1] = temp;
		}
	}
	if (B->entries[0] != 0) {
		float coeff = B->entries[0];
		for (int j = 1;j <= B->col;j++) {
			B->entries[j - 1] /= coeff;
		}
		for (int i = 2;i <= B->row;i++) {
			coeff = B->entries[(i - 1) * B->col];
			for (int j = 1;j <= B->col;j++) {
				B->entries[(i - 1) * B->col + j - 1] -= coeff * B->entries[j - 1];
			}
		}
	} else {
		float coeff = 0;
		for (int j = 1;j <= B->col;j++) {
			if (B->entries[j - 1] != 0 && coeff == 0) {
				coeff = B->entries[j - 1];
				B->entries[j - 1] = 1;
			} else if (B->entries[j - 1] != 0) {
				B->entries[j - 1] /= coeff;
			}
		}
	}
	struct Mat* B1 = removerow(B, 1);
	struct Mat* B2 = removecol(B1, 1);
	struct Mat* Be = rowechelon(B2);
	for (int i = 1;i <= Be->row;i++) {
		for (int j = 1;j <= Be->col;j++) {
			B->entries[i * B->col + j] = Be->entries[(i - 1) * Be->col + j - 1];
		}
	}
	freemat(B1);
	freemat(B2);
	freemat(Be);
	return B;
}
struct Mat* hconcat(struct Mat* A, struct Mat* B) {
	struct Mat* C = newmat(A->row, A->col + B->col, 0);
	int k = 0;
	for (int i = 1;i <= A->row;i++) {
		for (int j = 1;j <= A->col;j++) {
			C->entries[k] = A->entries[(i - 1) * A->col + j - 1];
			k++;
		}
		for (int j = 1;j <= B->col;j++) {
			C->entries[k] = B->entries[(i - 1) * B->col + j - 1];
			k++;
		}
	}
	return C;
}
struct Mat* vconcat(struct Mat* A, struct Mat* B) {
	struct Mat* C = newmat(A->row + B->row, A->col, 0);
	int k = 0;
	for (int i = 1;i <= A->row;i++) {
		for (int j = 1;j <= A->col;j++) {
			C->entries[k] = A->entries[(i - 1) * A->col + j - 1];
			k++;
		}
	}
	for (int i = 1;i <= B->row;i++) {
		for (int j = 1;j <= B->col;j++) {
			C->entries[k] = B->entries[(i - 1) * B->col + j - 1];
			k++;
		}
	}
	return C;
}
float norm(struct Mat* A) {
	float d = 0;
	int k = 0;
	for (int i = 1;i <= A->row;i++) {
		for (int j = 1;j <= A->col;j++) {
			d += A->entries[k] * A->entries[k];
			k++;
		}
	}
	d = sqrt(d);
	return d;
}
struct Mat* null(struct Mat* A) {
	struct Mat* RM = rowechelon(A);
	int k = RM->row;
	for (int i = RM->row;i >= 1;i--) {
		int flag = 0;
		for (int j = 1;j <= RM->col;j++) {
			if (RM->entries[(i - 1) * RM->col + j - 1] != 0) {
				flag = 1;
				break;
			}
		}
		if (flag) {
			k = i;
			break;
		}
	}
	struct Mat* RRM = submat(RM, 1, k, 1, RM->col);
	freemat(RM);
	int nn = RRM->col - RRM->row;
	if (nn == 0) {
		struct Mat* N = newmat(0, 0, 0);
		return N;
	}
	struct Mat* R1 = submat(RRM, 1, RRM->row, 1, RRM->row);
	struct Mat* R2 = submat(RRM, 1, RRM->row, 1 + RRM->row, RRM->col);
	freemat(RRM);
	struct Mat* I = eye(nn);
	struct Mat* T1 = multiply(R2, I);
	freemat(R2);
	struct Mat* R3 = scalermultiply(T1, -1);
	freemat(T1);
	struct Mat* T2 = triinverse(R1);
	freemat(R1);
	struct Mat* X = multiply(T2, R3);
	freemat(T2);
	freemat(R3);
	struct Mat* N = vconcat(X, I);
	freemat(I);
	freemat(X);
	for (int j = 1;j <= N->col;j++) {
		float de = 0;
		for (int i = 1;i <= N->row;i++) {
			de += N->entries[(i - 1) * N->col + j - 1] * N->entries[(i - 1) * N->col + j - 1];
		}
		de = sqrt(de);
		for (int i = 1;i <= N->row;i++) {
			N->entries[(i - 1) * N->col + j - 1] /= de;
		}

	}
	return N;
}

float innermultiply(struct Mat* a, struct Mat* b) {
	float d = 0;
	int n = a->row;
	if (a->col > n) {
		n = a->col;
	}
	for (int i = 1;i <= n;i++) {
		d += a->entries[i - 1] * b->entries[i - 1];
	}
	return d;
}
#endif