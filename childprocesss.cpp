#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    // Çocuk proses oluştur
    pid = fork();

    if (pid < 0) {
        // Hata oluştu
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Çocuk proses
        //excelp harici komut çalıştırır ve cat eklenmesi istendiği için ikisini beraber yazdım
        execlp("cat", "cat", "dosya.txt", NULL);
        // Eğer execlp başarılı olursa buraya gelmez
        perror("execlp");
        exit(1);
    } else {
        // parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Çocuk proses başarıyla tamamlandı, çıkış kodu: %d\n", WEXITSTATUS(status));
        } else {
            printf("Çocuk process sonlandı\n");
        }
    }

    return 0;
}

//ABORT

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) { // Çocuk proses
        FILE *fp = fopen("nonexistent_file.txt", "r");

        if (fp == NULL) {
            perror("fopen");
            abort(); // Hata durumunda zorla çıkış
        } else {
            fclose(fp);
            exit(0); // Başarılı çıkış
        }
    } else { // Ana proses
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Çocuk proses normal olarak sona erdi, çıkış kodu: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Çocuk proses bir sinyal nedeniyle sona erdi, sinyal numarası: %d\n", WTERMSIG(status));
        } else {
            printf("Çocuk proses beklenmedik şekilde sona erdi\n");
        }
    }

    return 0;
}

// ÇOKLU GÖREV

void create_file() {
    FILE *fp = fopen("output.txt", "w");
    if (fp == NULL) {
        perror("Dosya oluşturulamadı");
        exit(1);
    }
    fclose(fp);
}

void write_file() {
    FILE *fp = fopen("output.txt", "a");
    if (fp == NULL) {
        perror("Dosya yazılamadı");
        exit(1);
    }
    fclose(fp);
}

void read_file() {
    FILE *fp = fopen("output.txt", "r");
    if (fp == NULL) {
        perror("Dosya okunamadı");
        exit(1);
    }
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    fclose(fp);
}
// 3 tane oluşturalım
int main() {
    pid_t pid1, pid2, pid3;

    pid1= fork();
    if (pid1==0) {
        create_file();
        exit(0);
    }

    pid2= fork();
    if (pid2==0) {
        create_file();
        exit(0);
    }

    pid3= fork ();
    if (pid3==0) {
        create_file();
        exit(0);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    return 0;

}