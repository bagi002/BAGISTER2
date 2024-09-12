<h1 align="center" style="font-size: 48px;">BAGISTER 2</h1>


Ukratko o projektu i planu razvoja:
    Projekat prestavlja cilj razvoja malog robotskog automobila daljinski upravljivog
    putem dzojstika na velikim daljinama, ali u konacnici prvenstveno razvoj
    autonomnog vozila, integracijom senzora i aktuatora planira se postizanje djelimicno
    do potpuno autonomne voznje u buducem periodu.  
    Predstavlja svojevrsnog nasljediga prethodne verzije BAGISTER ali i poucen nekim
    iskustvima i znacajan iskorak kako u hardverskoj tako i softverskoj konfigursciji.  
    Sam projekat prestavlja dva odvojena uredjaja samog auta ali i nista manje bitnog
    aparata za upravljanje i andzor koji u krajnjoj fazi treba da ima modernu kontrolnu
    tablu ali i prenos slke koju snima kamera na samom vozilu.

Hardverska konfiguracija:  

    Aktutori:  
        -4x Kineski DC gerad motor  
        -2x L298N driver modul (palnirana zamjena sa novijim TB6612 DRV883 drajverima)  
    Upravljacko racunarski sistem:  
        -1x Arduino uno  
        -2x ESP32 (jedan na dzojstiku drugi na autu)  
    Senzori:  

    Napajanje:  
        -2x 9V Duracel alkalne baterije( planirana zamjena jacim litijumskim punjivim baterijama)  
    Ostala oprema:  
        -1x Aktivna zujalica  

Softverska konfiguracija:
    MotorDrajver (Arduino uno):

    Upravljacki sistem vozila (ESP32):

    Dzojstik (ESP32):

Istorija razvoja:

    V2.0.0:
        -Rezvijen osnovni skup drajverskih instrukcija za pokretanje i zaustavljanje vozila
        -Kreiran novi konfiguracioni model vozila
        -Uradjen niz testova motora na najnizem stepenu kontrole.
    V2.1.0
        -Razvijen komunikacioni protokoo ka sistemom za upravljanje motora (ESP -> ARDUINO)
        -Razvijen set naprednijih upravljackih komandi za rukovanje motorima
        -Razvijen u okviru samog drajvera sistem za prijem komande i odgovor na istu
    V2.2.0
        -Razvijen napredniji sistem komunikacije dzojstik <-> auto zasnovan na ESP-NOW
    V2.3.0
        -Razvijen novi sistem ocitavanja dzojstika sa novim sistemom ocitavanja osjetljivosti
        -Prvi put implementiran novi sistem za odredjivanje stepena skretanja vozila u kretanju
            kao i sistem koji obezbjedjuje da se ne odgovara na zahtjeve koje vozilo fizicki nemoze obezbjedit
        -Kreiran napredniji komunikacioni protokol za slanje komandi na vozilo
    V2.4.0
        -Unapredjen motor drajver da podrzi rolling kretanje odnosno motanje u pokretu
        -unapredjivanje veze izmedju dva uredjaja tako da podrzi dodatne podatke
        -Kreiran sistem prijema informacije na automobilu i slanje podataka drajveru motora
    V2.4.1
        -Ispravljena greska u drajveru vozila koja je nakon skretanja vozila izaazivala gresku pri 
         kretanju naprjed
    V2.5.0
        -Implementiran freeRTOS sistem na vozilu odnosno na glavnom mikrokokontroleru u sistemu
    