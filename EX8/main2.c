#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

// 1. 預設參數 (來自 image_d4a501.png, slide 51)
#define COUNTER_FILE "counter.bin"
#define MAX_LOTTO_NUM 7 // 每組的號碼數量 (6+1)
#define NUM_SET 5       // 總共 5 組 (包含 -- -- -- 的)

//
// 函數原型宣告 (C語言需要先宣告)
//
void init_file();
int get_counter();
void set_counter(int counter);
void do_lotto_main(int counter);
void print_lottofile(int num_set, int counter, char lotto_file[]);
void print_lotto_row(FILE* tmpfp, int n);
int num_in_numset(int num, int numset[], int Len);


//
// 主程式 (來自 image_d4a501.png, slide 51, *已修正邏輯*)
//
int main() {
    int counter;
   
    // 1. 初始化小本本 (image_d4a501.png, slide 50)
    init_file();
   
    // 2. 讀取小本本，得到 counter (image_d4a501.png, slide 50)
    counter = get_counter();
   
    // 3. 關鍵邏輯修正：將 counter + 1，
    //    這樣才會產生 lotto[00001].txt (而不是 lotto[00000].txt)
    counter++;
   
    printf("counter = %d\n", counter); // 印出當前 (新) 的期數
   
    // 4. 產生彩卷 (hw3程式) (image_d4a501.png, slide 50)
    do_lotto_main(counter);
   
    // 5. 把 counter++(新的值) 寫回小本本 (image_d4a501.png, slide 50)
    set_counter(counter);
   
    return 0;
}

//
// 步驟 (1): 設定帳簿 (來自 image_d4a53d.png, slide 53)
//
void init_file() {
    int write_array[1] = {0}; // 簡報 101 行
    FILE* fp = fopen(COUNTER_FILE, "r"); // 簡報 102 行
    if (fp == NULL) { // 簡報 103 行
        FILE* tmpfp = fopen(COUNTER_FILE, "wb+"); // 簡報 104 行
        fwrite(write_array, sizeof(int), 1, tmpfp); // 簡報 105 行
        fclose(tmpfp); // 簡報 106 行
    } else {
        fclose(fp); // 簡報 108 行
    }
}

//
// 步驟 (2): 讀取帳簿 (來自 image_d4a53d.png, slide 54)
//
int get_counter() {
    int read_array[1];
    FILE* tmpfp = fopen(COUNTER_FILE, "rb"); // 簡報 41 行
    fread(read_array, sizeof(int), 1, tmpfp); // 簡報 42 行
    fclose(tmpfp); // 簡報 43 行
    return read_array[0]; // 簡報 44 行
}

//
// 步驟 (4): 紀錄帳簿 (來自 image_d4a5d8.png, slide 62)
//
void set_counter(int counter) {
    int write_array[1];
    write_array[0] = counter; // 簡報 49 行
    FILE* tmpfp = fopen(COUNTER_FILE, "wb"); // 簡報 50 行
    fwrite(write_array, sizeof(int), 1, tmpfp); // 簡報 51 行
    fclose(tmpfp); // 簡報 52 行
}

//
// 步驟 (3): 輸出彩卷 (主體) (來自 image_d4a53d.png, slide 55)
//
void do_lotto_main(int counter) {
    char lotto_file[32]; // 簡報 87 行
    int num_set = 0; // 簡報 88 行
   
    snprintf(lotto_file, 32, "lotto[%05d].txt", counter); // 簡報 89 行
   
    // 根據簡報 90-94 行，補全使用者介面
    printf("歡迎光臨長庚樂透彩選號系統\n");
    printf("請問您要購買幾組樂透彩 (1-5): ");
    scanf("%d", &num_set); // 簡報 92 行

    // 檢查使用者輸入，防止超過 5 組
    if (num_set > NUM_SET) {
        printf("輸入錯誤，最多只能購買 %d 組。\n", NUM_SET);
        num_set = NUM_SET;
    } else if (num_set < 1) {
        printf("輸入錯誤，至少要購買 1 組。\n");
        num_set = 1;
    }

    print_lottofile(num_set, counter, lotto_file); // 簡報 93 行
   
    printf("已為您購買 %d 組樂透組合，檔案已輸出至 %s\n", num_set, lotto_file);
}

//
// 步驟 (3.1): 輸出彩卷 (檔案寫入) (來自 image_d4a57c.png, slide 56)
//
void print_lottofile(int num_set, int counter, char lotto_file[]) {
    time_t curtime;
    time(&curtime); // 簡報 67 行
   
    // 根據簡報 68 行，srand 應該在 main 函數中呼叫一次就好
    // 但為求完全符合簡報，我們將它保留在此
    srand(time(0));
   
    FILE* tmpfp = fopen(lotto_file, "w+"); // 簡報 69 行
   
    fprintf(tmpfp, "======== lotto649 =========\n"); // 簡報 70 行
    fprintf(tmpfp, "=======+ No.%05d +========\n", counter); // 簡報 71 行
   
    // 簡報 72 行: ctime() 會自帶換行符，%.24s 可移除換行
    fprintf(tmpfp, "= %.24s =\n", ctime(&curtime));
   
    // 簡報 74 行: 這裡的 MAX_LOTTO_NUMSET 應該是 NUM_SET (5)
    for (int i = 0; i < NUM_SET; i++) {
        if (i < num_set) { // 簡報 75 行
            // 簡報 76 行，但我們需要印出 [n]:
            fprintf(tmpfp, "[%d]: ", i + 1);
            print_lotto_row(tmpfp, i + 1);
        } else {
            // 簡報 78 行
            fprintf(tmpfp, "[%d]: -- -- -- -- -- -- --\n", i+1);
        }
    }
   
    fprintf(tmpfp, "======== csie@CGU =========\n"); // 簡報 81 行
    fclose(tmpfp); // 簡報 83 行
}


//
// 步驟 (3.1.x): 處理單一一組彩卷 (來自 image_d4a57c.png, slide 57)
//
void print_lotto_row(FILE* tmpfp, int n) {
    int numset[MAX_LOTTO_NUM]; // 簡報 22 行
   
    //
    // 步驟 (3.1.1): 找到一組彩卷 (來自 image_d4a5a1.png, slide 58)
    //
    // 產生前 6 個 1-69 的號碼
    for (int i = 0; i < MAX_LOTTO_NUM - 1; ) { // 簡報 26 行
        int num = (rand() % 69) + 1; // 簡報 27 行
        if (num_in_numset(num, numset, i)) { // 簡報 28 行
            continue; // 簡報 29 行
        } else {
            numset[i] = num; // 簡報 31 行
            i++; // 簡報 32 行
        }
    }
    // 產生第 7 個 1-10 的號碼
    for (int i = 0; i < 1; ) { // 簡報 36 行
        int num = (rand() % 10) + 1; // 簡報 37 行
        if (num_in_numset(num, numset, MAX_LOTTO_NUM - 1)) { // 簡報 38 行
            continue; // 簡報 39 行
        } else {
            numset[MAX_LOTTO_NUM - 1] = num; // 簡報 41 行
            i++; // 簡報 42 行
        }
    }
   
    //
    // 步驟 (3.1.2): 號碼排序 (前6個) (來自 image_d4a5d8.png, slide 61)
    //
    for (int i = 0; i < MAX_LOTTO_NUM - 1; i++) { // 簡報 47 行
        for (int j = 0; j < i; j++) { // 簡報 48 行
            if (numset[j] > numset[i]) { // 簡報 49 行
                int temp = numset[j]; // 簡報 50 行
                numset[j] = numset[i]; // 簡報 51 行
                numset[i] = temp; // 簡報 52 行
            }
        }
    }
   
    //
    // 步驟 (3.1.3): 印出一組彩卷號碼 (來自 image_d4a5d8.png, slide 61)
    //
    for (int i = 0; i < MAX_LOTTO_NUM; i++) { // 簡報 58 行
        fprintf(tmpfp, "%02d ", numset[i]); // 簡報 59 行
    }
    fprintf(tmpfp, "\n"); // 簡報 61 行
}


//
// 步驟 (3.1.1.1): 判斷是否有重複號碼 (來自 image_d4a5a1.png, slide 58)
//
int num_in_numset(int num, int numset[], int Len) {
    int ret = 0; // 簡報 11 行
    for (int i = 0; i < Len; i++) { // 簡報 12 行
        if (num == numset[i]) { // 簡報 13 行
            ret = 1; // 簡報 14 行
            break; // 簡報 15 行
        }
    }
    return ret; // 簡報 18 行
}

