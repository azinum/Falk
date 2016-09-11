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
