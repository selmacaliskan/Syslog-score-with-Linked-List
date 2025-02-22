# Syslog Bağlı Liste Programı

Bu proje, Linux işletim sisteminde syslog verilerini bağlı liste veri yapısını kullanarak gösteren bir C programıdır. Program, `/var/log/syslog` dosyasını okuyarak her bir log girişini bağlı listeye ekler ve ardından bu listeyi terminalde yazdırır.

## Kullanılan Yapılar ve Fonksiyonlar

### Yapılar

- `SyslogEntry`: Syslog kayıtlarını tutmak için kullanılan bir yapıdır. Bu yapı, zaman damgası (`timestamp`), log seviyesi (`logLevel`), mesaj (`message`) ve bir sonraki kayıt için bir işaretçi (`next`) içerir.

### Fonksiyonlar

- `createSyslogEntry`: Yeni bir syslog girişi oluşturur ve bellekte yer ayırır.
- `addSyslogEntry`: Yeni bir syslog girişini bağlı listenin başına ekler.
- `printSyslogEntries`: Bağlı listedeki tüm syslog girişlerini yazdırır.
- `freeSyslogEntries`: Bağlı listedeki tüm syslog girişleri için ayrılan belleği serbest bırakır.
- `main`: Programın ana fonksiyonudur. Syslog dosyasını okur, her bir log girişini bağlı listeye ekler ve ardından bu listeyi yazdırır ve belleği serbest bırakır.

## Kullanılan Bağlı Liste Türü

Bu programda kullanılan bağlı liste türü, **tek yönlü bağlı liste** (singly linked list) olarak adlandırılır. Her bir düğüm (node), bir sonraki düğümün adresini tutan bir işaretçi içerir. Bu tür bağlı listede, her düğüm yalnızca bir sonraki düğümü işaret eder ve liste sonuna kadar bu şekilde devam eder.

## Programın Çalıştırılması

### Adım 1: Kaynak Kodu Kaydetme

Öncelikle, aşağıdaki C kodunu bir dosya olarak kaydedin. Örneğin, `syslog_list.c` olarak kaydedin.

```c
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
```

### Adım 2: Derleme

C programını derlemek için bir C derleyicisi kullanmanız gerekecek. Yaygın olarak kullanılan derleyici `gcc`'dir. Terminalde aşağıdaki komutu çalıştırarak programınızı derleyebilirsiniz:

```sh
gcc -o syslog_list syslog_list.c
```

### Adım 3: Çalıştırma

Programı çalıştırmak için derlenen dosyayı çalıştırmanız gerekecek. Ancak syslog dosyasına erişim genellikle root yetkileri gerektirir. Bu nedenle, programı root yetkileri ile çalıştırmalısınız:

```sh
sudo ./syslog_list
```

### Sonuçları Görme

Program çalıştırıldığında, `/var/log/syslog` dosyasındaki log girişlerini bağlı listeye ekler ve bu listeyi terminalde yazdırır. Çıktıyı terminalde görebilirsiniz. Her log girişi, zaman damgası, log seviyesi ve mesaj içeriği ile birlikte ekrana yazdırılacaktır.

### Önemli Notlar!

- **Yetki Gereksinimi:** Syslog dosyasına erişim genellikle yalnızca root kullanıcı tarafından yapılabilir. Bu nedenle, programı çalıştırırken `sudo` komutunu kullanmanız gerekecek.
- **Bağlı Liste Kullanımı:** Program, her bir log girişini bağlı listeye ekler ve tüm listeyi yazdırır. Bu, log verilerini dinamik olarak yönetmek için etkili bir yöntemdir.
- **Bellek Yönetimi:** Program, bağlı liste için ayrılan belleğin serbest bırakılmasını sağlar. Bu, bellek sızıntılarını önlemek için önemlidir.

## Lisans

Bu proje MIT Lisansı altında lisanslanmıştır. Daha fazla bilgi için `LICENSE` dosyasına bakınız.
