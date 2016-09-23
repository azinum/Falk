# Loggbok


### V35 - 2016/9/1

Nu har jag börjat skapa mitt egna programmering/skriptingspråk.
Jag har alltid vilja göra ett eget språk. Det första språket jag använde var Python, det var en stor insperation.
Det var nästan magiskt hur man kunde skriva in exempelvis ett matemetiskt tal och så räkna Python ut talet åt mig.

När man gör ett programmeringsspråk så måste utföra en hel del steg i form av Lexing, Parsing och sedan utveckla den virtuella maskinen.
Jag kommer förklara mer senare när jag kommer igång.
I denna stund så har jag gjort en enkel lexer. Men, vad är en lexer?
Ganska enkelt, användaren skriver in en sträng från konsolen och lexern gör om strängen till data strukturer.
Varje data struktur består av ett tecken / ord och en beskrivning av tecknet.
En lexer är som ett riktigt språks lexikon, leta upp ett ord och hitta betydelsen.

Till nästa vecka så kommer jag börja med parser.




### V36 - 2016/9/9

Jag har denna vecka påbörjat att göra en parser. En parsers uppgift är att omvandla en ingång av tecken till någon form av träd.
Trädet är definerat av språkets grammatik. Grammatiken bestämmer sen syntaxen.
Varje språk har sin egna grammatik.
Språk har detta för att beskriva hur syntaxen ska vara formad och för att datorn ska lätt kunna behandla det användaren vill åstadkomma.

##### Exempel
Vi kanske vill göra en enkel uträkning.
Hur gör vi för att parsa detta?
``` ruby
=> 3 + 2
```
Resultatet på detta exempel blir:
``` ruby
=> 3 2 +
```
Varför?
Jo, för att datorn ska kunna räkna ut detta så använder vi oss av en stack.
När vi ser ett nummer, då lägger vi in nummret på stacken.
Om vi ser en operator, då räknar vi ut värden som ligger på stacken.
Såhär funkar en virtuell maskin i princip.

Min parser kommer använda sig av the Shunting Yard algorithim, eller järnvägsalgoritmen och en hemmagjord grammatik.
Den använder sig av operatörer och icke-operatörer. Varje operatör har en prioritet och associativitet.
Med hjälp av en prioritet och associativitet så kan man lätt sortera ingången av kod till ett syntax träd.




### V37 - 2016/9/16

Denna vecka har jag börjat med att göra grammatik till mitt språk.
Jag har en lista med regler som parsern kollar igenom och gör en viss felsökning och lite manipulationer på ingången av kod.
Detta steg, parsingen och grammatiken är det svåraste att utveckla när man skapar ett språk.
Jag har en idé om hur man kan göra detta.

Såhär tänkte jag forma språkets regler:
``` ruby
(IF, EXPR, BODY | END);
(WHILE, EXPR, BODY | END);
(FUNC, EXPR, BODY | END);
```
När parsern upptäcker if ordet, då kollar den igenom reglerna för if.
Så, efter if, då måste man använda en expression, efter expression måste man antingen ha en 'body' eller 'end'.
Man markerar eller med den binära operatorn OR (|).
Allt man gör är att använda så kallat flaggor eller mask.

Exempelvis:
``` ruby
BODY    = 00001000
END     = 00000100
```
När man gör OR operatorn över dom två nummrerna så får vi ett resultat:
``` ruby
    00001000
OR  00000100
    00001100
```

För att kolla om det finns en flagga i ett nummer så använder man operatorn AND (&).
``` ruby
    00001100
AND 00000100
    00000100
```
Här ser vi att det matchar.


Jag tänker till nästa vecka börja utveckla den virtuella maskinen.
Just nu ska jag forska mera om parser och hur man formar ett språks syntax.


### V38 - 2016/9/23

Jag har utvecklat en enkel Virtuell Maskin denna vecka. Den kan exekvera instruktioner som använderen skriver in i konsolen.
Dock så har jag stött på en hel del buggar. Buggarna var 'memory errors', d.v.s minnesfel.
När användaren skriver in kod till konsolen så har jag gjort något fel när jag kopierar minne från en funktion till en annan.
Jag ska spåra felen och fixa dom så snabbt jag kan.
När jag har fixat felen så ska jag lägga till instruktioner så att jag kan spara variabler och redigera variabler.
Efter det så ska jag fortsätta med parsern. Jag ska leta upp lösningar för hur man gör en parser.
Jag kan göra en parser men, problemet är, jag vill göra en parser som gör det lätt för en att lägga till nya funktioner.
