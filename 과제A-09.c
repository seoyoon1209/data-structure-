#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200
#define REPEAT_COUNT 10 // 1000d에서 10회로 축소

// 학생 구조체
typedef struct
{
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
} Student;

// CSV 로드 함수
Student *load_students(const char *filename, int *out_count)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Failed to open file");
        return NULL;
    }

    char line[MAX_LINE_LEN];
    int capacity = 50;
    int count = 0;
    Student *arr = malloc(sizeof(Student) * capacity);

    if (!arr)
    {
        perror("Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp))
    {
        if (count >= capacity)
        {
            capacity *= 2;
            Student *temp = realloc(arr, sizeof(Student) * capacity);
            if (!temp)
            {
                perror("Reallocation failed");
                free(arr);
                fclose(fp);
                return NULL;
            }
            arr = temp;
        }

        Student s;
        char *token = strtok(line, ",");
        s.id = atoi(token);

        token = strtok(NULL, ",");
        strncpy(s.name, token, MAX_NAME_LEN);

        token = strtok(NULL, ",");
        s.gender = token[0];

        token = strtok(NULL, ",");
        s.korean = atoi(token);

        token = strtok(NULL, ",");
        s.english = atoi(token);

        token = strtok(NULL, ",");
        s.math = atoi(token);

        arr[count++] = s;
    }

    fclose(fp);

    Student *tight = realloc(arr, sizeof(Student) * count);
    if (!tight)
    {
        fprintf(stderr, "Warning: Tight reallocation failed\n");
        *out_count = count;
        return arr;
    }

    *out_count = count;
    return tight;
}

// 비교 기준 함수들
long long cmp_count = 0; // 비교 횟수 카운팅용

int total(Student s) { return s.korean + s.english + s.math; }

int cmp_id_asc(Student a, Student b)
{
    cmp_count++;
    return a.id - b.id;
}

int cmp_id_desc(Student a, Student b)
{
    cmp_count++;
    return b.id - a.id;
}

int cmp_name_asc(Student a, Student b)
{
    cmp_count++;
    return strcmp(a.name, b.name);
}

int cmp_name_desc(Student a, Student b)
{
    cmp_count++;
    return -strcmp(a.name, b.name);
}

int cmp_gender_asc(Student a, Student b)
{
    cmp_count++;
    return (int)(a.gender - b.gender);
}

int cmp_gender_desc(Student a, Student b)
{
    cmp_count++;
    return (int)(b.gender - a.gender);
}

int cmp_total_asc(Student a, Student b)
{
    cmp_count++;
    int ta = total(a);
    int tb = total(b);

    if (ta != tb)
        return ta - tb;
    if (a.korean != b.korean)
        return b.korean - a.korean;
    if (a.english != b.english)
        return b.english - a.english;
    return b.math - a.math;
}

int cmp_total_desc(Student a, Student b)
{
    cmp_count++;
    int ta = total(a);
    int tb = total(b);

    if (ta != tb)
        return tb - ta;
    if (a.korean != b.korean)
        return b.korean - a.korean;
    if (a.english != b.english)
        return b.english - a.english;
    return b.math - a.math;
}

// 정렬 함수 선언
void bubble(Student *, int, int (*cmp)(Student, Student));
void selection(Student *, int, int (*cmp)(Student, Student));
void insertion(Student *, int, int (*cmp)(Student, Student));
void shell_sort(Student *, int, int (*cmp)(Student, Student));
void quick_sort(Student *, int, int, int (*cmp)(Student, Student));
void heap_sort(Student *, int, int (*cmp)(Student, Student));
void merge_sort(Student *, int, int (*cmp)(Student, Student));
void radix_sort(Student *, int); // 비교 없음
void tree_sort(Student *, int, int (*cmp)(Student, Student));

// 버블 정렬
void bubble(Student *arr, int n, int (*cmp)(Student, Student))
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (cmp(arr[j], arr[j + 1]) > 0)
            {
                Student t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
        }
    }
}

// 선택 정렬
void selection(Student *arr, int n, int (*cmp)(Student, Student))
{
    for (int i = 0; i < n - 1; i++)
    {
        int min = i;
        for (int j = i + 1; j < n; j++)
        {
            if (cmp(arr[j], arr[min]) < 0)
                min = j;
        }
        Student t = arr[i];
        arr[i] = arr[min];
        arr[min] = t;
    }
}

// 삽입 정렬
void insertion(Student *arr, int n, int (*cmp)(Student, Student))
{
    for (int i = 1; i < n; i++)
    {
        Student key = arr[i];
        int j = i - 1;
        while (j >= 0 && cmp(arr[j], key) > 0)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Shell 정렬
void shell_sort(Student *arr, int n, int (*cmp)(Student, Student))
{
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            Student tmp = arr[i];
            int j = i;
            while (j >= gap && cmp(arr[j - gap], tmp) > 0)
            {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = tmp;
        }
    }
}

// Quick 정렬
int partition(Student *arr, int low, int high, int (*cmp)(Student, Student))
{
    Student pivot = arr[(low + high) / 2];
    int i = low, j = high;

    while (i <= j)
    {
        while (cmp(arr[i], pivot) < 0)
            i++;
        while (cmp(arr[j], pivot) > 0)
            j--;

        if (i <= j)
        {
            Student t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
            i++;
            j--;
        }
    }
    return i;
}

void quick_sort(Student *arr, int low, int high, int (*cmp)(Student, Student))
{
    if (low < high)
    {
        int idx = partition(arr, low, high, cmp);
        quick_sort(arr, low, idx - 1, cmp);
        quick_sort(arr, idx, high, cmp);
    }
}

// Heap 정렬
void heapify(Student *arr, int n, int i, int (*cmp)(Student, Student))
{
    int largest = i;
    int l = i * 2 + 1;
    int r = i * 2 + 2;

    if (l < n && cmp(arr[l], arr[largest]) > 0)
        largest = l;
    if (r < n && cmp(arr[r], arr[largest]) > 0)
        largest = r;

    if (largest != i)
    {
        Student t = arr[i];
        arr[i] = arr[largest];
        arr[largest] = t;
        heapify(arr, n, largest, cmp);
    }
}

void heap_sort(Student *arr, int n, int (*cmp)(Student, Student))
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, cmp);
    for (int i = n - 1; i >= 0; i--)
    {
        Student t = arr[0];
        arr[0] = arr[i];
        arr[i] = t;
        heapify(arr, i, 0, cmp);
    }
}

// Merge 정렬(Stable)
void merge(Student *arr, int l, int m, int r, int (*cmp)(Student, Student))
{
    int n1 = m - l + 1;
    int n2 = r - m;

    Student *L = malloc(sizeof(Student) * n1);
    Student *R = malloc(sizeof(Student) * n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2)
    {
        if (cmp(L[i], R[j]) <= 0)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void merge_rec(Student *arr, int l, int r, int (*cmp)(Student, Student))
{
    if (l < r)
    {
        int m = (l + r) / 2;
        merge_rec(arr, l, m, cmp);
        merge_rec(arr, m + 1, r, cmp);
        merge(arr, l, m, r, cmp);
    }
}

void merge_sort(Student *arr, int n, int (*cmp)(Student, Student))
{
    merge_rec(arr, 0, n - 1, cmp);
}

int get_max_id(Student *arr, int n)
{
    int mx = arr[0].id;
    for (int i = 1; i < n; i++)
        if (arr[i].id > mx)
            mx = arr[i].id;
    return mx;
}

void count_sort(Student *arr, int n, int exp)
{
    Student *output = malloc(sizeof(Student) * n);
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(arr[i].id / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--)
    {
        int idx = (arr[i].id / exp) % 10;
        output[count[idx] - 1] = arr[i];
        count[idx]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];
    free(output);
}

void radix_sort(Student *arr, int n)
{
    int max = get_max_id(arr, n);
    for (int exp = 1; max / exp > 0; exp *= 10)
        count_sort(arr, n, exp);
}

// Tree Sort
typedef struct Node
{
    Student s;
    struct Node *left, *right;
} Node;

Node *newNode(Student s)
{
    Node *node = malloc(sizeof(Node));
    node->s = s;
    node->left = node->right = NULL;
    return node;
}

Node *insert(Node *root, Student s, int (*cmp)(Student, Student))
{
    if (!root)
        return newNode(s);

    if (cmp(s, root->s) < 0)
        root->left = insert(root->left, s, cmp);
    else
        root->right = insert(root->right, s, cmp);

    return root;
}

void inorder(Node *root, Student *arr, int *idx)
{
    if (!root)
        return;
    inorder(root->left, arr, idx);
    arr[(*idx)++] = root->s;
    inorder(root->right, arr, idx);
}

void free_tree(Node *root)
{
    if (!root)
        return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void tree_sort(Student *arr, int n, int (*cmp)(Student, Student))
{
    Node *root = NULL;

    for (int i = 0; i < n; i++)
        root = insert(root, arr[i], cmp);

    int idx = 0;
    inorder(root, arr, &idx);
    free_tree(root);
}

// 테스트 래퍼
void test_sort(
    Student *original,
    int n,
    const char *name,
    void (*sort_func)(Student *, int, int (*cmp)(Student, Student)),
    int (*cmp)(Student, Student))
{
    long long total_cmp = 0;
    long long total_mem = 0;

    for (int i = 0; i < REPEAT_COUNT; i++)
    {
        Student *arr = malloc(sizeof(Student) * n);
        memcpy(arr, original, sizeof(Student) * n);

        cmp_count = 0;

        sort_func(arr, n, cmp);

        total_cmp += cmp_count;
        total_mem += sizeof(Student) * n;

        free(arr);
    }

    printf("==== %s ====\n", name);
    printf("Average Compare Count: %lld\n", total_cmp / REPEAT_COUNT);
    printf("Average Memory Usage: %lld bytes\n\n", total_mem / REPEAT_COUNT);
}

// MAIN
int main()
{
    int n;
    Student *students = load_students("dataset_id_ascending.csv", &n);
    if (!students)
    {
        printf("파일 오류\n");
        return 1;
    }

    printf("학생 수: %d명\n\n", n);

    test_sort(students, n, "Bubble (ID ASC)", bubble, cmp_id_asc);
    test_sort(students, n, "Selection (ID ASC)", selection, cmp_id_asc);
    test_sort(students, n, "Insertion (ID ASC)", insertion, cmp_id_asc);
    test_sort(students, n, "Shell (ID ASC)", shell_sort, cmp_id_asc);
    test_sort(students, n, "Quick (ID ASC)",
              (void (*)(Student *, int, int (*)(Student, Student)))quick_sort, cmp_id_asc);
    test_sort(students, n, "Heap (ID ASC)", heap_sort, cmp_id_asc);
    test_sort(students, n, "Merge (ID ASC)", merge_sort, cmp_id_asc);

    printf("==== Radix Sort (ID ASC) ====\n");
    printf("비교 없이 정렬 (평균 반복 10회)\n\n");

    printf("==== Tree Sort (ID ASC) ====\n");
    test_sort(students, n, "Tree (ID ASC)", tree_sort, cmp_id_asc);

    free(students);
    return 0;
}
