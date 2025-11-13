#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n;
    scanf("%d", &n); // 從輸入讀取要產生的樂透組數
    srand(1);        // 固定亂數種子

    FILE *fp = fopen("lotto.txt", "w");
    fprintf(fp, "======== lotto649 ========\n");
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(fp, "======= %s %d %d =======\n", 
            (t->tm_mon == 2) ? "March" : "Month", // 只是示範
            t->tm_mday, t->tm_year + 1900);

    for (int i = 1; i <= 5; i++) {
        fprintf(fp, "[%d]: ", i);
        if (i <= n) {
            int lotto[6];
            for (int j = 0; j < 6; j++) lotto[j] = rand() % 69 + 1;
            for (int j = 0; j < 6; j++) fprintf(fp, "%02d ", lotto[j]);
            fprintf(fp, "%02d\n", rand() % 10 + 1); // 特別號
        } else {
            fprintf(fp, "-- -- -- -- -- -- --\n");
        }
    }

    fprintf(fp, "======== csie@CGU ========\n");
    fclose(fp);
    return 0;
}
