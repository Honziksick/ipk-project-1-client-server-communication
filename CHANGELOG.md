# CHANGELOG

**Autor:** Jan Kalina (`xkalinj00`)

**Předmět:** *IPK – Počítačové komunikace a sítě* <br>
**Akademický rok:** *2024/2025*

---

## Implementovaná funkcionalita

- Objektově orientovaná architektura s využitím návrhových vzorů (např. _facade_), dědičnosti a polymorfismu.
- Skenování **TCP** a **UDP** portů na zadaných **IP** adresách nebo _hostname_.
- Možnost zadání jednotlivých portů, rozsahů portů nebo jejich kombinací.
- Parsování vstupních parametrů pomocí robustního parseru argumentů založeném na knihovně **CLI11**.
- Barevný výpis aktivních síťových rozhraní s detailními informacemi (nad rámec zadání), 
pokud není zadáno konkrétní rozhraní.
- Automatická detekce verze **IP** (**IPv4** nebo **IPv6**).
- Podpora volitelného parametru `--wait` pro nastavení časového limitu odpovědi.
- Generování náhodných zdrojových portů a sekvenčních čísel pro bezpečné síťové operace.
- Kvalitní mechanismus zpracování výjimek včetně jejich centrálního zachytávání a jednotného výstupu.
- Barevně zvýrazněný a strukturovaný výstup informací a chybových hlášek.
- Robustní systém jednotkových testů postavených na frameworku Google Test, včetně mockování systémových
volání.
- Rozsáhlý Makefile s podporou režimu pro odevzdání a režimu pro vývoj, včetně stavby testů, generování 
dokumentace a správy závislostí.
- Centralizované řízení aplikace pomocí fasád `OmegaAppFacade` a `ScannerController`, které výrazně 
zjednodušují spouštění a řízení skenovacího procesu.
- Komplexní validace vstupních argumentů, včetně kontroly formátu **IP** adres a _hostname_ pomocí regulárních
výrazů.
- Optimalizace zadávání portů, jako jsou odstranění duplicit, sloučení souse dících portů do rozsahů, převod 
rozsahů reprezentujících jediný port do jednoduché formy a řazení portů.
- Využití _raw socketů_ a knihovny _libnet_ k přesnému sestavování a odesílání **TCP**/UDP paketů, 
včetně manipulace s **IP** a **TCP**/**UDP** hlavičkami.

## Známá omezení

- Program musí být spuštěn s administrátorskými právy (_root_), jelikož využívá _raw sockety_ pro odesílání paketů.
- _Raw sockety_ v Linuxu nezachytí _loopbackové_ pakety na jiných než _loopback_ rozhraních.
  - Pakety směřující na _loopback_ adresy (např. `localhost`, `127.0.0.1`) mohou být viditelné v nástroji `tcpdump`,který je zachytává 
    na linkové vrstvě pomocí `libpcap`, ale nejsou doručeny _raw socketům_ navázaným na jiné síťové rozhraní (např. `enp0s3`).
  - Linuxové jádro takové pakety pravděpodobně zahazuje na úrovni _IP stacku_.
- Časové prodlevy a _timeouty_ jsou závislé na konfiguraci sítě, což může ovlivnit přesnost detekce stavu portů, 
zejména v silně filtrovaných síťových prostředích.

V současné době nejsou známa další omezení nebo problémy. Aplikace byla důkladně testována v popsaném testovacím 
prostředí a během testování nebyla objevena žádná další kritická omezení.
