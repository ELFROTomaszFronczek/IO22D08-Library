<div class="content "><p>Podczas przeszukiwania internetu znalazłem całkiem ciekawą płytkę rozwojową która może posłużyć za dobrą bazę do sterownika automatyki. Jest to płytka o nazwie IO22D08 wiadomej chińskiej produkcji.</p>
<p style="text-align: center;"><img class="cimg" src="img/IO22D08.jpg" alt=""></p>
<p style="text-align: justify;">&nbsp;Niestety jedyne znalezione oprogramowanie tej płytki po długich poszukiwaniach było dalekie od zadowalającego. dlatego na jego bazie postanowiłem napisać swoją "bibliotekę"&nbsp; do obsługi płytki.</p>
<p style="text-align: justify;">Płytka przystosowana jest do montażu z płytką Adruino Pro Mini lecz nic nie szkodzi aby np testowo podłączyć też zamiast niej bardziej popularne Arduino Nano.&nbsp; W tym celu zwieramy kawałkami przewodów wyjście D10 do A7 i D11do A6 i wkładamy jak na rysunku:</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: center;"><img class="cimg" src="https://elfro.pl/img/IO22D08-NANO.jpg" alt="" width="600" height="351"></p>
<p style="text-align: justify;">Oprogramowanie.</p>
<p style="text-align: justify;">W kodzie źródłowym zawarte są wszystkie definicje i procedury niezbędne do obsługi pytki.&nbsp;</p>
<p style="text-align: justify;">Na samym początku musimy zdefiniować jakie Arduino wkładamy. Jeżeli jest to Nano pozostawiamy linię <code><span style="color: #339966;">#define</span> NANO_AS_MINI </code>a jeżeli jest to oryginalne MINI - usuwamy ją.</p>
<p style="text-align: justify;">obsługa funkcji jest bardzo prosta.</p>
<p><strong>1. Obsługa przycisków K1..K4 i wejść IN1..IN8</strong></p>
<p style="text-align: justify;">W pętli <code>loop()</code> zamieszczamy funkcje:</p>
<pre style="text-align: justify;">&nbsp; readINPUTS();<br>&nbsp; readKEYS();</pre>
<p style="text-align: justify;">Pierwsza odczytuje stan wejść druga stan przycisków. Program filtruje odczytane dane eliminując drgania zestyków wprowadzając 50ms pętlę na eliminację drgań dlatego te funkcje powinny wykonywać się dość często. Gdybyśmy dodali je do timera np używając flexiTimer2 jak w LCD mógłby powstać problem z wykrywaniem i detekcją zbocza (Naciśniecie, Puszczenie) - jeżeli nie jest nam to potrzebne wykorzystujemy tylko stan 1/0 oczywiście można to umieścić w procedurze flexiTimera.</p>
<p style="text-align: justify;">odczytane dane mogą mieć wartości</p>
<ul>
<li style="text-align: justify;">0 - wejście niepodłączone lub przycisk nie naciśnięty . Wartość IO_LOW</li>
<li style="text-align: justify;">1 - wejście zawarte do masy lub naciśnięty przycisk. Wartość IO_HIGH</li>
<li style="text-align: justify;">2 - moment zwierania wejścia / naciskania przycisku&nbsp; - zbocze narastające. Wartość IO_RISING</li>
<li style="text-align: justify;">3 - moment rozwierania wejścia / puszczenia przycisku&nbsp; - zbocze opadające. Wartość IO_FALLING</li>
</ul>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">wartości odczytujemy z tablic inValues i keysValues np:</p>
<pre style="text-align: justify;">&nbsp;byte in1=inValues[0]; <br>&nbsp;byte key1=keysValues[0]; </pre>
<p style="text-align: justify;">gdzie wartość 0 oznacza pierwsze wejście(IN1) lub przycisk(K1)&nbsp; itd kolejne aż do 7 dla wejść i do 3 dla przycisków 7 - wejście IN8. 3- przycisk K4.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;"><strong>2. Obsługa LCD</strong></p>
<p style="text-align: justify;">&nbsp;LCD włączamy lub wyłączamy&nbsp; definiując 1 lub 0 w tej stałej:</p>
<pre><span style="color: #5e6d03;">#define</span> <span style="color: #000000;">USE_LCD</span> <span style="color: #000000;">1</span>

</pre>
<p style="text-align: justify;">Jeżeli płytka ma obsługiwać wyświetlacz LCD musimy go często odświeżać gdyż segmenty cyfr nie wyświetlają się na stałe - wymagałoby to odrobinę więcej elektroniki co wiadomo nie jest na rękę w masowej produkcji "po taniości".</p>
<p style="text-align: justify;">Wyświetlacz można obsługiwać w pętli loop lub używając timera np FlexiTimer2.h, którego użyłem w przykładzie.</p>
<p style="text-align: justify;">Wyświetlacz może wyświetlać cyfry oraz symulować w przybliżeniu alfabet i niektóre znaki.</p>
<p style="text-align: justify;">Niestety kropka nie jest w pełni obsługiwana. Jedynie dostępny jest dwukropek dla drugiego segmentu.</p>
<p style="text-align: justify;">Segmenty numerowane są od 0. 0 to pierwszy segment 1 drugi itd..</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Dostępne funkcje:</p>
<p style="text-align: justify;"><code>clearLCD();</code> - czyści ekran .</p>
<p style="text-align: justify;"><br><code>setLCDdigit(segment, value, dots);</code> - ustawia w wybranym segmencie wartość liczbową.</p>
<p style="text-align: justify;">segment - numer segmentu zaczynając od 0</p>
<p style="text-align: justify;">value - może mieć wartość od 0 do 45 gdzie początkowe wartości to cyfry a kolejne litery. Na końcu są niektóre pozostałe znaki.</p>
<p style="text-align: justify;">dots - parametr opcjonalny - wyświetla dwukropek na środku.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;"><code>setLCDtext(char[]);</code> - wypisuje tekst</p>
<p style="text-align: justify;">parametrem jest tekst do wpisania np <code>setLCDtext("TEST");</code></p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;"><code>setLCDTime(hour, minute,dots );</code> - wyświetla godzinę i minutę lub minutę i sekunde wg uznania oraz opcjonalnie dwukropek</p>
<p style="text-align: justify;"><br><code>setLCDbyInt(intValue);</code> - wyświetla liczbę całkowitą od -999 do 9999<br><br>Można także zdefiniować własne znaki. Aby to zrobić najpierw zobaczmy który segment za co odpowiada:</p>
<p style="text-align: justify;"><code>&nbsp;&nbsp;&nbsp; --A--</code><br><code>&nbsp;&nbsp;&nbsp; F---B</code><br><code>&nbsp;&nbsp;&nbsp; --G--</code><br><code>&nbsp;&nbsp;&nbsp; E---C</code><br><code>&nbsp;&nbsp;&nbsp; --D--</code></p>
<p style="text-align: justify;"><code style="line-height: 0;">&nbsp; &nbsp; __&nbsp; __&nbsp;&nbsp; __&nbsp; __<br/>&nbsp;&nbsp; |__||__|.|__||__|<br/>&nbsp;&nbsp; |__||__|'|__||__|</code></p>
<p style="text-align: justify;">Każdy z 4 elementów wyświetlacza&nbsp; składa się z 7 segmentów kolejno A,B,C,D,E,F,G</p>
<p style="text-align: justify;">gdzie A,B,C to kolejne bity maski dla kodowanego znaku. A to najmniej ważny bit- nr.0&nbsp; G - najbardziej nr.6 przykładowo w zapisie binarnym będzie to wyglądać tak 0bGFEDCBA</p>
<p style="text-align: justify;">np aby ustawić na wyświetlaczu cyfrę 1 musimy zapalić segmenty B i C da to cyfrę 0b00000110</p>
<p style="text-align: justify;">maska jest negacją tej cyfry gdyż aktywny segment wyświetlacza wysterowywany jest w negacji - świeci gdy jest ustawiony bit na 0</p>
<p style="text-align: justify;">np:&nbsp;&nbsp; mask= ~( 0b GFEDCBA);</p>
<p style="text-align: justify;">Ósmy bit jest odpowiedzialny za kropkę, a w zasadzie za dwukropek i jest aktywny tylko dla drugiego segmentu. Stąd kropka nie jest obsługiwana.<br><br><code>setCustomChar(segment, mask, dots);</code> - definiuje własny znak według maski w wybranym segmencie z opcjonalnym dwukropkiem.<br><br><code>byte setCustomChar(segment,&nbsp; a,&nbsp; b,&nbsp; c, d,&nbsp; e,&nbsp; f,&nbsp; g ,&nbsp; dots );</code> - definiuje własny znak gdzie możemy wybrać które segmenty abcdefg&nbsp; mają być włączone 0- wyłączony 1- włączony.<br>Dodatkowo funkcja zwraca obliczoną maskę np do późniejszego użytku.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;"><strong>3. Obsługa przekaźników</strong></p>
<p style="text-align: justify;">Do obsługi przekaźników zdefiniowałem kilka funkcj:</p>
<p style="text-align: justify;"><code>setRelay(relayNo);</code> - włącza wybrany przekaźnik. Numeracja zaczyna się od 1. Przekaźnik pierwszy&nbsp; wybieramy 1 drugi wybieramy 2</p>
<p style="text-align: justify;"><code>resetRelay(relayNo);</code> - wyłącza wybrany przekaźnik.<br><br><code>setRelayAndOffRest(relayNo);</code>&nbsp; - włącza wybrany przekaźnik i wyłącza pozostałe<br><br><code>byte getRelay(relayNo);</code> - zwraca stan wybranego przekaźnika 0 lub 1<br><br><code>clearRelays();</code>&nbsp; - wyłącza wszystkie przekaźniki</p>
<p style="text-align: justify;">&nbsp;Jeżeli numeracja od 1 nam nie odpowiada i chcemy numerować od 0 nie ma problemu. Należy w pliku <code>lcdAndRelays.h</code> znaleźć i usunąć wszystkie linie z kodem <code>relayNo--;</code></p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">&nbsp;</p>
<hr>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Kod źródłowy można pobrać stąd: <a href="download/IO22D08.zip">IO22D08.zip</a> rozmiar ok. 7kB</p></div>
