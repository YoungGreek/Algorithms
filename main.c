#include <stdlib.h>
#include <stdio.h>

typedef struct point {
    long long x;
    long long y;
} point;

point min_point;

// очистка памяти
void destroy_memory(point *points_array) {
    free(points_array);
}

// правильность ввода
int check_input(long long *N) {
    if (scanf("%lld", N) != 1) {
        return 0;
    }
    if (*N < 0 || *N > 100000) {
        printf("bad number of points");
        return 0;
    }
    if (*N == 0) {
        return 0;
    }
    return 1;
}

// для поиска первой подходящей точки
void swap(point *points, int ind) {
    point buff = points[0];
    points[0] = min_point;
    points[ind] = buff;
}

// векторное произведение
int orientation(point p, point q, point r) {
    long double tmp_qpy = (q.y - p.y);
    long double tmp_rqx = (r.x - q.x);
    long double tmp_qpx = (q.x - p.x);
    long double tmp_rqy = (r.y - q.y);
    long double val = (tmp_qpy * tmp_rqx) - (tmp_qpx * tmp_rqy);

    if (val == 0) return 0; // Коллинеарные
    return (val > 0) ? 1 : 2; // 1 - По часовой стрелке, 2 - Против часовой стрелке
}

// для сортировки по полярному углу

int compare(const void *vp1, const void *vp2) {
    point *p1 = (point *) vp1;
    point *p2 = (point *) vp2;

    int orient = orientation(*p1, *p2, min_point);
    if (orient == 0) {
        return !(p1->y < p2->y || (p1->y == p2->y && p1->x < p2->x));
    }

    return (orient == 2) ? -1 : 1;
}

void Graham_Algorithm(point *points_array, int N, int index) {
    swap(points_array, index);

    // Сортировка по полярному углу
    qsort(points_array + 1, N - 1, sizeof(point), compare);

    if (N < 3) {
        for (int i = 0; i < N; i++) {
            printf("%lld %lld\n", points_array[i].x, points_array[i].y);
        }
        return;
    }

    // Нахождение минимальной выпуклой оболочки
    int top = 0;

    for (int i = 1; i < N; i++) {
        while (top > 0 && (orientation(points_array[top - 1], points_array[top], points_array[i]) != 2)) {
            top--;
        }
        points_array[++top] = points_array[i];
    }

    // Вывод координат выпуклой оболочки
    for (int i = 0; i < top + 1; i++) {
        printf("%lld %lld\n", points_array[i].x, points_array[i].y);
    }
}


int main(void) {
    long long N;
    int ind = 0;
    if (check_input(&N) == 0) {
        return EXIT_SUCCESS;
    }
    point *points_array = (point *) (malloc(sizeof(point) * N));
    min_point.x = -__LONG_LONG_MAX__;
    min_point.y = __LONG_LONG_MAX__;

    int points_counter = 0;

    // Поиск начальной точки (min y, max x)
    for (int i = 0; i < N; i++) {
        long long point_x, point_y;
        if (scanf("%lld %lld", &point_x, &point_y) != 2) {
            printf("bad number of lines");
            destroy_memory(points_array);
            return 0;
        }
        if (min_point.y >= point_y) {
            if (min_point.y == point_y) {
                if (min_point.x > point_x) {
                    min_point.y = point_y;
                    min_point.x = point_x;
                    ind = i;
                }
            } else {
                min_point.y = point_y;
                min_point.x = point_x;
                ind = i;
            }

        }
        points_array[i].x = point_x;
        points_array[i].y = point_y;
        points_counter += 1;
    }
    if (points_counter != N) {
        printf("bad number of lines");
        destroy_memory(points_array);
        return EXIT_SUCCESS;
    }

    Graham_Algorithm(points_array, N, ind);
    destroy_memory(points_array);
    return EXIT_SUCCESS;
}
