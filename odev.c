#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

#define mak_ad 10
#define mak_sifre 12

#define normal_kapasite 5
#define sigara_kapasite 5

struct Musteriler
{
    char isim[mak_ad];
    char sifre[mak_sifre];
};

struct Musteriler musteri[50];
int musteri_sayisi = 0;

struct Rezervasyonlar
{
    char isim[mak_ad];
    int saat;
    int sigara;
    int bebek;
};
int rezervasyon_sayisi = 0;

struct Rezervasyonlar rezervasyon[10]; // eðer saat 24 ile 6 arasý ise sýfýrla

//                              <<<VERÝ OKUMA ÇÝZME>>>

void dosya_yaz(char isim[], char sifre[])
{ // musteri sayýsý , musteri bilgileri file yazdýr
    FILE *fp = fopen("veri.txt", "a"); //veri.txt açar yoksa oluþturur "a" amacý append üstüne yazma
    if (fp)
    {
        fprintf(fp, "%s %s\n", isim, sifre); // yazdýrma
        fclose(fp);
    }
    return;
}

void dosya_bilgi_al()
{ // kümeyi doldur program açýlýnca
    FILE *fp = fopen("veri.txt", "r");
    if (!fp)
        return;
        //for loop iþlemedi bulunan musteri sayisi bilinmior, scanf bilgi alýr
    while (fscanf(fp, "%s %s", musteri[musteri_sayisi].isim, musteri[musteri_sayisi].sifre) == 2)
    {
        musteri_sayisi++;
    }
    fclose(fp);
}

//                      <<<giriþ iþlemleri>>>

int giris_index_bul(const char *isim) // þifre bulma olayý
{
    int i;
    for (i = 0; i < musteri_sayisi; i++)
    {
        if (strcmp(musteri[i].isim, isim) == 0)
        {
            return i;
        }
    }
    return -1;
}

char *giris_isim(char *isim)
{
    printf("Lütfen isminizi tanýmlayýn\n");

    fgets(isim, mak_ad, stdin);
    isim[strcspn(isim, "\n")] = '\0'; // fgetsin aldýðý sondaki bitiriciyi silmek

    printf("Ýsminizi %s olarak algýladýk doðru mudur? (Doðru ise 1 yanlýþ ise 2 tuþlayabilirsiniz)\n", isim);

    int sayi;
    char kurban[16]; // geçici depolama

    fgets(kurban, sizeof(kurban), stdin);
    kurban[strcspn(kurban, "\n")] = '\0';
    sayi = atoi(kurban);

    if (sayi != 1)
    {
        giris_isim(isim);
    }
    return isim;
}

char *giris_sifre(char *sifre)
{
    printf("Lütfen þifre tanýmlayýn ve ya þifrenizi girin\n");

    fgets(sifre, mak_sifre, stdin);
    sifre[strcspn(sifre, "\n")] = '\0';

    printf("Þifrenizi %s olarak algýladýk doðru mudur? (Doðru ise 1 yanlýþ ise 2 tuþlayabilirsiniz)\n", sifre);

    int sayi;
    char kurban[16];

    fgets(kurban, sizeof(kurban), stdin);
    kurban[strcspn(kurban, "\n")] = '\0';
    sayi = atoi(kurban);

    if (sayi != 1)
    {
        giris_sifre(sifre);
    }
    return sifre;
}

void giris_yeni(char isim[], char sifre[])
{
    strcpy(musteri[musteri_sayisi].isim, isim);
    strcpy(musteri[musteri_sayisi].sifre, sifre);

    dosya_yaz(isim, sifre);

    musteri_sayisi++;
    return;
}

int giris()
{
    char isim[mak_ad];
    giris_isim(isim);

    char sifre[mak_sifre];
    giris_sifre(sifre);

    int index = giris_index_bul(isim);
    if (index != -1)
    {
        if (strcmp(musteri[index].sifre, sifre) != 0)
        { // depðilse
            index = -1;
        }
    }
    else
    { // yeni müþteri
        giris_yeni(isim, sifre);
        return 0;
    }

    if (index == -1)
    {
        printf("Þifre veya Ýsim yanlýþ lütfen tekrar deneyiniz.\n");
        giris();
    }
    else
    {
        printf("Giriþ baþarýlý, rezervasyona yönlendiriliyorsunuz...\n");
        return 0;
    }
}

//                          <<REZERVASYON ÝÞLEMLERÝ>>

int rezervasyon_saat_musait(int saat, int sigara)
{
    int doluluk = 0;
    int i;
    for (i = 0; i < rezervasyon_sayisi; i++)
    {
        if (rezervasyon[i].saat == saat)
        {
            doluluk++;
        }
    }
    // int kapasite = if { (sigara == 1) sigara_kapasite} else {normal_kapasite};
    int kapasite = (sigara == 1) ? sigara_kapasite : normal_kapasite;
    if (doluluk <= kapasite)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void rezervasyon_saat_ayarlama(int *rezervasyon_saati, int sigara_tercihi)
{
    printf("Lütfen rezervasyon saatinizi girin. Örnek: 0130 (24 saatlik format olarak girin rakamlar için baþýna 0 ekleyin): ");

    char kurban[5];

    fgets(kurban, sizeof(kurban), stdin);
    kurban[strcspn(kurban, "\n")] = '\0';
    *rezervasyon_saati = atoi(kurban);

    if (rezervasyon_saat_musait(*rezervasyon_saati, sigara_tercihi) == 0)
        rezervasyon_saat_ayarlama(rezervasyon_saati, sigara_tercihi);
    return;
}

void rezervasyon_sigara_ayarlama(int *sigara_tercihi)
{
    printf("Sigara içilen alan mý, içilmeyen alan mý tercih edersiniz? (1: Ýçilen, 2: Ýçilmeyen): ");

    char kurban[4];

    fgets(kurban, sizeof(kurban), stdin);
    kurban[strcspn(kurban, "\n")] = '\0';
    *sigara_tercihi = atoi(kurban);

    if (*sigara_tercihi == 1 || *sigara_tercihi == 2)
    {
        return;
    }
    else
    {
        rezervasyon_sigara_ayarlama(sigara_tercihi);
    }
}

void rezervasyon_bebekli_ayarlama(int *bebek_sandalyesi)
{
    printf("Bebek sandalyesi ister misiniz? (1: Evet, 2: Hayýr): ");
    char kurban[16];

    fgets(kurban, sizeof(kurban), stdin);
    kurban[strcspn(kurban, "\n")] = '\0';
    *bebek_sandalyesi = atoi(kurban);

    if (*bebek_sandalyesi == 1 || *bebek_sandalyesi == 2)
    {
        return;
    }
    else
    {
        rezervasyon_bebekli_ayarlama(bebek_sandalyesi);
    }
}

int rezervasyon_onay(int saat, int sigara, int bebekli)
{
    printf("Rezervasyonunuz %04d, sigara %s alan, bebek sandalyesi %s olarak kaydedilmiþtir.\n",
           saat,
           (sigara == 1) ? "içilen" : "içilmeyen",
           (bebekli == 1) ? "var" : "yok");

    printf("Rezervasyonu onaylýyor musunuz? (1: Evet, 2: Hayýr): ");

    int onay;
    char kurban[16];

    fgets(kurban, sizeof(kurban), stdin);
    kurban[strcspn(kurban, "\n")] = '\0';
    onay = atoi(kurban);
    return onay;
}

//                              <MAÝN>

int main()
{
    setlocale(LC_ALL, "Turkish");

    dosya_bilgi_al();
    
    while (1)
    {
        printf("Restoranýn sesli ve tuþlu komutlu randevu sistemine hoþgelidniz.\n");

        giris();

        printf("Lütfen, rezervasyon seçiminizi yapýn.\n");
        int sigara_tercihi;
        int rezervasyon_saati;
        int bebek_sandalyesi;

        rezervasyon_sigara_ayarlama(&sigara_tercihi);
        rezervasyon_saat_ayarlama(&rezervasyon_saati, sigara_tercihi);
        rezervasyon_bebekli_ayarlama(&bebek_sandalyesi);

        int onay = 0;
        onay = rezervasyon_onay(rezervasyon_saati, sigara_tercihi, bebek_sandalyesi);
        if (onay == 1)
        {
            printf("<<<Rezervasyonunuz onaylandý.>>>\n");
        }
        else
        {
            printf("Ýþlem iptal edildi.\n");
        }
        printf("Bizi tercih ettiðiniz için teþekkür ederiz. Ýyi günler dileriz.\n\n");
    }
}

