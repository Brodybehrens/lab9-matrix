#include <bits/stdc++.h>
using namespace std;

using Matrix = vector<vector<long long>>; 


static int cellWidth(const Matrix &M) {
    long long mx = 0;
    for (const auto &row : M)
        for (auto v : row) mx = max(mx, llabs(v));
    int w = 1;
    while (mx >= 10) { mx /= 10; ++w; }
    return max(w, 2); 
}

static void printMatrix(const Matrix &M, const string &label = "Matrix") {
    cout << "\n" << label << " (" << M.size() << "x" << (M.empty()?0:M[0].size()) << "):\n";
    if (M.empty()) { cout << "<empty>\n"; return; }
    int w = cellWidth(M) + 1; 
    for (const auto &row : M) {
        for (auto v : row) cout << setw(w) << v;
        cout << '\n';
    }
}

static Matrix add(const Matrix &A, const Matrix &B) {
    size_t n = A.size();
    Matrix C(n, vector<long long>(n, 0));
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

static Matrix multiply(const Matrix &A, const Matrix &B) {
    size_t n = A.size();
    Matrix C(n, vector<long long>(n, 0));
    for (size_t i = 0; i < n; ++i)
        for (size_t k = 0; k < n; ++k)
            for (size_t j = 0; j < n; ++j)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

static pair<long long,long long> diagonalSums(const Matrix &M) {
    size_t n = M.size();
    long long mainSum = 0, secondarySum = 0;
    for (size_t i = 0; i < n; ++i) {
        mainSum += M[i][i];
        secondarySum += M[i][n - 1 - i];
    }
    return {mainSum, secondarySum};
}

static bool swapRows(Matrix &M, size_t r1, size_t r2) {
    size_t n = M.size();
    if (r1 >= n || r2 >= n) return false;
    swap(M[r1], M[r2]);
    return true;
}

static bool swapCols(Matrix &M, size_t c1, size_t c2) {
    size_t n = M.size();
    if (n == 0 || c1 >= n || c2 >= n) return false;
    for (size_t i = 0; i < n; ++i) swap(M[i][c1], M[i][c2]);
    return true;
}

static bool updateCell(Matrix &M, size_t r, size_t c, long long val) {
    size_t n = M.size();
    if (r >= n || c >= n) return false;
    M[r][c] = val; return true;
}


static bool readTwoMatricesFromFile(const string &filename, Matrix &A, Matrix &B) {
    ifstream fin(filename);
    if (!fin) return false;
    long long Nll; if (!(fin >> Nll) || Nll <= 0) return false;
    size_t N = static_cast<size_t>(Nll);
    A.assign(N, vector<long long>(N, 0));
    B.assign(N, vector<long long>(N, 0));
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            if (!(fin >> A[i][j])) return false;
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            if (!(fin >> B[i][j])) return false;
    return true;
}

static void showMenu() {
    cout << "\nChoose an option:\n";
    cout << " 1) Print matrices A and B\n";
    cout << " 2) A + B\n";
    cout << " 3) A x B\n"; 
    cout << " 4) Diagonal sums (choose A or B)\n";
    cout << " 5) Swap rows (choose A or B)\n";
    cout << " 6) Swap columns (choose A or B)\n";
    cout << " 7) Update a cell (choose A or B)\n";
    cout << " 0) Quit\n> ";
    cout << flush; // ensure prompt is visible before reading input
}


static Matrix* chooseMatrix(Matrix &A, Matrix &B) {
    while (true) {
        cout << "Use matrix (A/B)? ";
        string s; if (!(cin >> s)) return nullptr;
        if (s == "A" || s == "a") return &A;
        if (s == "B" || s == "b") return &B;
        cout << "Please enter A or B.\n";
    }
}

int main(int argc, char* argv[]) {
    


    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    Matrix A, B;
    if (!readTwoMatricesFromFile(argv[1], A, B)) {
        cerr << "Error: failed to read matrices from file '" << argv[1] << "'.\n";
        cerr << "Expected format: N then N*N for A then N*N for B (whitespace-separated).\n";
        return 2;
    }

    cout << "Loaded matrices from '" << argv[1] << "'.\n";

    int choice = -1;
    while (true) {
        showMenu();
        if (!(cin >> choice)) break;
        if (choice == 0) break;
        switch (choice) {
            case 1: {
                printMatrix(A, "A");
                printMatrix(B, "B");
                break;
            }
            case 2: {
                Matrix C = add(A, B);
                printMatrix(C, "A + B");
                break;
            }
            case 3: {
                Matrix C = multiply(A, B);
                printMatrix(C, "A × B");
                break;
            }
            case 4: {
                Matrix *M = chooseMatrix(A, B);
                if (!M) break;
                auto [mainD, secD] = diagonalSums(*M);
                cout << "Main diagonal sum = " << mainD << '\n';
                cout << "Secondary diagonal sum = " << secD << '\n';
                
                long long uniqueTotal = mainD + secD;
                if (M->size() % 2 == 1) uniqueTotal -= (*M)[M->size()/2][M->size()/2];
                cout << "Total (counting center once) = " << uniqueTotal << '\n';
                break;
            }
            case 5: {
                Matrix *M = chooseMatrix(A, B);
                if (!M) break;
                size_t r1, r2; cout << "Row indices r1 r2 (0-based): ";
                if (!(cin >> r1 >> r2)) { cout << "Invalid input.\n"; cin.clear(); break; }
                if (!swapRows(*M, r1, r2)) cout << "Out of bounds.\n"; else printMatrix(*M, "After row swap");
                break;
            }
            case 6: {
                Matrix *M = chooseMatrix(A, B);
                if (!M) break;
                size_t c1, c2; cout << "Column indices c1 c2 (0-based): ";
                if (!(cin >> c1 >> c2)) { cout << "Invalid input.\n"; cin.clear(); break; }
                if (!swapCols(*M, c1, c2)) cout << "Out of bounds.\n"; else printMatrix(*M, "After column swap");
                break;
            }
            case 7: {
                Matrix *M = chooseMatrix(A, B);
                if (!M) break;
                size_t r, c; long long v; cout << "r c newValue: ";
                if (!(cin >> r >> c >> v)) { cout << "Invalid input.\n"; cin.clear(); break; }
                if (!updateCell(*M, r, c, v)) cout << "Out of bounds.\n"; else printMatrix(*M, "After update");
                break;
            }
            default:
                cout << "Unknown option.\n";
        }
    }
// ios::sync_with_stdio(false);
// cin.tie(nullptr);
    cout << "Goodbye!\n";
    return 0;
}

