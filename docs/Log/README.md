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


### V39 - 2016/9/30

Denna vecka lyckades jag fixa minnesfelen som skedde i den Virtuella Maskinen.
Det berodde på att jag inte gav tillräckligt med minne till en viss variabel. Det är fixat nu.
I virtuella maskinen lade jag till två instruktioner. En instruktion, den försöker leta upp en variabel, om den hittar variabeln då pushar man den på stacken.
Om variabeln inte hittas, då skapas en ny variabel med värdet ``` null```, d.v.s. inget värde.
Den andra instruktionen som lades till var tilldelningsoperatorn. Man kan ge en variabel det värdet man vill ha med hjälp av tilldelningsoperatorn ```=```.

Jag har forskat en del denna vecka om hur man formar ett språks syntax. Jag vet nu hur jag ska forma mitt språk. Jag ska forma mitt språk med en [Recursive descent parser](https://en.wikipedia.org/wiki/Recursive_descent_parser).
Det är tillräkligt enkelt att man kan göra denna parser för hand. Stora språk som [Ruby](https://www.ruby-lang.org/en/) eller [Python](https://www.python.org) använder en parser generator.
Vad en [Parser Generator](https://en.wikipedia.org/wiki/Comparison_of_parser_generators) gör är att, man kan definiera en rad regler och sen så kompileras sitt program efter dom reglerna.
Detta är någonting jag absolut inte vill göra. Jag vill ha full kontroll över mitt språk.
Till nästa vecka så ska jag ha gjort en enkel parser.


### V40 - 2016/10/8

Under denna vecka så har jag inte jobbat så mycket med själva programmeringen.
Jag fastnade när jag skulle implementera parsern. Jag hade problem med att skaffa en strategi för hur jag skulle forma parsern.
Nu har jag suttit och skrivit ner olika strategier och lösningar för problemet. Jag har kommit på hur jag ska göra.
Som jag nämnde förra veckan så kommer jag använda mig av en sorts recursive parsing (funktionen ropar sig själv). Parsern läser strömmen med tecken och följer gramatiska regler som är definierat sedan innan.
Den kollar framåt i strömmen med tecken och kollar så att det stämmer överens med reglerna.
Parsern använder sig också av en sorts produktion. Om den upptäcker två speciella tecken efter varandra så kommer den producera ett tredje tecken.

``` ruby

	A  B  C  D
a   f  g  h  i
b   j  k  l  m
c   n  o  p  q
d   r  s  t  u

```
Detta fungerar som en multiplikationstabell.
``` ruby
	A + c = A + c + n

	D + a = D + a + i
```
Detta är användbart i språket då vi behöver en extra instruktion / tecken efter ett uttryck eller liknande.
Man kan använda detta när man ska ropa en funktion.
``` ruby
test(5 + 5);
```
För att ropa en funktion så behöver man en ```identifier``` (namn) och sedan en ```tuple``` (lista med argument kan man säga).
Då producerar den uttrycket på detta sett:
``` ruby
test(5 + 5) callf;
```
```callf``` betyder call function eller ropa funktion. Man behöver göra så här för att alla parenteser kommer försvinna, då finns det inget sett att se om det är ett funktionsanrop eller inte.


### V41 - 2016/10/16

Jag har denna vecka lyckats med att skapa en 'produktion' funktion.
Funktionen är till för att skapa extra instruktioner där det behövs. Som jag beskrev förra veckan, denna funktion är viktig.
Jag kommer denna vecka påbörja den större delen av parsern. Parser kommer generera ett linjärt träd med noder.
Parsern kommer ha ett optimiserings steg där den tar bort onödiga noder och liknande optimiseringar.
När man väl har detta linjära träd kan man generera ``` bytecode ```. Vad är bytecode?
Bytecode i Falk kommer vara en lista med [pointers](http://www.programiz.com/c-programming/c-pointers). En pointer pekar på en adress i ram.
På dessa adresser så finns antingen en instruktion eller ett värde.
Vad gör bytecode? Som sagt är bytecode en lista med instruktioner och dom blir exekverade av Virtuella Maskinen.
Här har vi ett exempel på ett program, kompilerat till bytecode:

``` ruby
PUSHK 5
PUSHK 9
ADD
EXIT
```

Vad var det som hände i detta program? ``` PUSHK ``` pushar en konstant på ``` stack ```, denna instruktion tar ett argument.
Så, man pushar två konstanter på ``` stack ``` och sedan adderar dom och pushar resultatet.

### V42 - 2016/10/23

Jag har denna vecka jobbat mycket med parsen. Nu kan man kolla ifall en syntax är korrekt eller inte.

Här är ett exempel:

<img src="https://raw.githubusercontent.com/Azinum/Falk/master/docs/Log/images/Syntax-Check.png" width="256">

Jag har en funktion som heter ``` check_next ```. Den kollar frammåt i ingången av symboler och gämför med regler som är definierat i språket. Om syntaxen är korrekt, då kan vi fortsätta. Vi använder block som vi har kollat upp i ingången och sedan parsar dom i vilken ordning grammatiken har bestämt.

Detta är en regel:

``` C
typedef struct Parse_rule {
    int type,   /* if, while, +, - */
        asso,   /* x(x) or (x)x */
        prec,   /* operator priority */
        *rule,  /* array of what can follow this type */
        *prio,  /* priority in which order a rule is parsed */
        rule_size;  /* how many items in rule */
} Parse_rule;
```
``` type ``` är för vilken typ vi ska parsa för, det kan vara ```if``` (om vilkor gör detta) eller ```
 while ``` (medans detta vilkor gör detta) o.s.v. ``` asso, prec ``` är till för operatörer, [associativity](https://en.wikipedia.org/wiki/Associative_property) och [operator precedence](https://en.wikipedia.org/wiki/Order_of_operations). För i vilken ordning operatörer ska pushas till resultat produktionen.
 ``` *rule ``` är en lista med var som är tillåtet att följa efter denna typ (vilka symboler som är tillåtna att vara efter denna symbol). Exempelvis så får ``` if ``` statement se ut såhär:

``` C
if (1);

if (1) {...}
```

``` *prio ``` är för vilken ordning block av symboler ska bli hanterade av parsern.

Nästa steg när jag har fixat parsern så ska man sedan generera ``` bytecode ``` som jag skrev lite om förra veckan.

Hur ska jag exempelvis göra om jag ska parsa en ``` while ``` loop?

``` C

a = 5;
while (a < 100) {
   a += 1;
}

```

Resultatet jag vill åstadkomma i parsern är detta:

``` C
a 5 = end
a 100 < while {
   a 1 += end
}

```

While loop ska ha instruktioner som ``` goto ``` som går till början av loopen när man nått slutet av body ``` {...} ```.
Jag vill ha detta resultat:

``` Ruby

1    pushi "a"		# push identifier to stack
3    pushk 5		# push constant to stack
4    equals_assign	# assign value
5    end		      # end expression (pop stack top)
7    pushi "a"
9    pushk 100
10   less_than		# less than comparison operator
12   while 21		# while instruction, if true: skip one instruction, else: use next instruction value to jump out of scope
14   pushi "a"
16   pushk 1
17   add_assign	# add and assign value
18   end
20   goto 6	      # goto start of loop
21   exit	      # exit program

```

Det är detta jag jobbar mot att klara av. Jag kommer stöta på en hel del problem, men dom är till för att lösas.
