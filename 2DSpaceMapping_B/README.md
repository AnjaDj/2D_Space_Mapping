# AUTONOMNO VOZILO - PROJEKTNI ZADATAK
Ovaj repozitorijum sadži dokumentaciju potrebnu za izradu projektnog zadataka iz predmeta Računarska elektronika/Mikrokontrolerski sistemi na Elektrotehničkom fakultetu u Banjoj Luci. Na projekatu rade tri studenta na četvrtoj godini. Cilj projekta jeste implementacija sistema za autonomnu vožnju koji može bezbijedno i efikasno upravljati vozilom. Sistem se nadograđuje na [Joy-car](https://joy-it.net/en/products/mb-joy-car) sa specificiranim zahtjevima.

# Specifikacija projektnog zadataka 
### Upravljačka PCB ploča
Potrebno je realizovati PCB pločicu koja ispunjava sljedeće zahtjeve: 
* PCB pločica može biti maksimalno dvoslojna.
* Koristi se SMD tip komponenti
* Minimalna širina veze prilikom rutiranja iznosi 0.254 mm, a minimalno rastojanje između komponenata 0.1 mm.
* Korišćenje mikrokontrolera (MCU) tipa PIC24FJ64GAxx za upravljanje i kontrolu sistema.
* Podrška za komunikaciju putem UART, SPI i I2C interfejsa za povezivanje različitih komponenti.
* Integracija Wi-Fi modula za bežičnu komunikaciju i daljinsko upravljanje.
* Upotreba već postojećeg baterijskog napajanja.
* Budzet za kupovinu komponenata od 20 do 30 dolara.

### IR Distance Click senzor
* Kretanje vozila na osnovu očitavanja senzora
* Implementacija 2D mapiranja prostora oko vozila na osnovu očivatanja senzora.

## Aplikacija
* Razvoj aplikacije za prikazivanje okoline oko vozila.
* Mogućnost pokretanja i zaustavljanja vozila putem aplikacije, 2D prikaz reljefa.
   
## Projektovanje štampane ploče
* Šema 
![kompletna_sema](https://github.com/codeandrelax/2DSpaceMapping_B/assets/74966262/43537a65-98cc-4a8a-b9fb-845c5a769ed6)

* Layout  
  ![image](https://github.com/codeandrelax/2DSpaceMapping_B/assets/127748379/3bb24229-8b67-4b08-92db-80bd839c3b21)
  ![back_cu](https://github.com/codeandrelax/2DSpaceMapping_B/assets/74966262/6e51d18d-6f20-47ca-a177-1033e38a085d)
  ![image](https://github.com/codeandrelax/2DSpaceMapping_B/assets/127748379/1006eb37-11cb-4f9b-a70c-6e451aa7aa0a)


* 3D model
  ![3d_model](https://github.com/codeandrelax/2DSpaceMapping_B/assets/74966262/4429c304-3d05-4c8f-8538-2fbbc5953749)
 ![3d_model2](https://github.com/codeandrelax/2DSpaceMapping_B/assets/74966262/8a7f46ab-b6f3-4677-9ab6-3f6122012498)


## Realizacija algoritma kretanja vozila

Potpuno integrisano samoupravljajuće vozilo Joy Car funkcioniše na taj način da se IR click senzor vozila najprije centrira tako da skenira prostor ispred sebe i vozilo se počinje kretati ravno, to je jedan od pet mogućih pravaca kretanja vozila. Nakon što vozilo prilikom kretanja naiđe na prepreku koja se nalazi na udaljenosti od 15cm, zaustavlja se i skenira se prostor oko vozila, tako što se okreće IR Distance Click senzor koji je postavljen na servo motoru koji se okreće za ugao od 45 stepeni u desnu stranu, odnosno lijevu. Nakon što na osnovu vrijednosti napona koju senzor generiše odredimo u kojem smjeru se prepreka nalazi najdalje, vozilo se počinje kretati u tom pravcu. Ukoliko su vrijednosti očitavanja senzora veće, tada se prepreka nalazi bliže vozilu i obratno. U slučaju da se prepreka nalazi jako blizu tada autić zbog širine točkova nije u mogućnosti da nesmetano skrene u željenom pravcu pa postoji i dodatni smijer kretanja, unazad. Tada autić najprije vozi nekoliko centimetara unazad, a zatim skrene u pravcu za koji je utvrdio da je najpovoljniji.  

Na slici ispod je prikazan primjer simulacije kretanja autića u lavirintu. 

![image](https://github.com/codeandrelax/2DSpaceMapping_B/assets/127748379/21f9cb5a-c9a5-4a76-ac15-6dfa36d3f7a8)  
  
Na gifu ispod prikazano je kretanje autića kroz lavirint:

![gif](https://github.com/codeandrelax/2DSpaceMapping_B/assets/127748379/cfee5903-df70-46f6-a8a7-c8ad88093a85)
