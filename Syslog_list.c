#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Syslog girdi yapılandırması
//tip tanımlaması
typedef struct SyslogEntry {
    char timestamp[20];//zaman damgası
    char logLevel[10];//log seviyesi(INFO,EROR vb.)
    char message[256];//log mesajı
    struct SyslogEntry* next;//sonraki düğüm(node) adresi
} SyslogEntry;

// Fonksiyon da yeni bir syslog girdisi oluşturulur.
SyslogEntry* createSyslogEntry(char* timestamp, char* logLevel, char* message) {
    SyslogEntry* newEntry = (SyslogEntry*)malloc(sizeof(SyslogEntry));
    strcpy(newEntry->timestamp, timestamp);
    strcpy(newEntry->logLevel, logLevel);
    strcpy(newEntry->message, message);
    newEntry->next = NULL;
    return newEntry;
}

// Fonksiyonda syslog girdisi listeye eklenir.
void addSyslogEntry(SyslogEntry** head, char* timestamp, char* logLevel, char* message) {
    SyslogEntry* newEntry = createSyslogEntry(timestamp, logLevel, message);
    newEntry->next = *head;
    *head = newEntry;
}

// Fonksiyon tüm syslog girdilerini yazdırır.
void printSyslogEntries(SyslogEntry* head) {
    SyslogEntry* current = head;
    while (current != NULL) {
        printf("%s %s %s\n", current->timestamp, current->logLevel, current->message);
        current = current->next;
    }
}

// Fonskiyon syslog için hafızada ayrılmış alanı serbest bırakıyor.
void freeSyslogEntries(SyslogEntry* head) {
    SyslogEntry* current = head;
    SyslogEntry* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

// Main fonksiyonunda syslog okunur ve girdiler bağlı listede depolanır.
int main() {
    SyslogEntry* syslogList = NULL;
    FILE* fp = fopen("/var/log/syslog", "r");
    if (fp == NULL) {
        perror("Failed to open syslog file");
        return EXIT_FAILURE;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        char timestamp[20], logLevel[10], message[256];
        // Syslog biçimini varsayarak: "timestamp logLevel message"
        sscanf(line, "%19s %9s %[^\n]", timestamp, logLevel, message);
        addSyslogEntry(&syslogList, timestamp, logLevel, message);
    }

    fclose(fp);

    // printSyslogEntries(syslog girdilerini yazmak) isimli fonksiyonu çağırma.
    printSyslogEntries(syslogList);

    // freeSyslogEntries(Syslog girdilerini serbest bırakma) isimli fonksiyonu çağırma.
    freeSyslogEntries(syslogList);
    
    // stdlib.h kütüphanesinde tanımlı bir makrodur(genel tanımlama).
	//Anlamı: Program başarılı bir şekilde çalıştı ve sorunsuz bir şekilde sonlandı.
    return EXIT_SUCCESS;
}
