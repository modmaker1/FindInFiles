
; 
; Copyright (c) 2011-2020 Ilya Lyutin
; 
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
; 
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
; 
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.
; 

.386
.MODEL FLAT, C

OPTION CASEMAP:		NONE
OPTION PROLOGUE:	NONE
OPTION EPILOGUE:	NONE

.data
ASSUME CS: NOTHING

;/////////////////////////////////////////////////////////////////////////////

PUBLIC __tp1250_toupper
__tp1250_toupper:

	db 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h
	db 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh
	db 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h
	db 018h, 019h, 01Ah, 01Bh, 01Ch, 01Dh, 01Eh, 01Fh
	db 020h, 021h, 022h, 023h, 024h, 025h, 026h, 027h
	db 028h, 029h, 02Ah, 02Bh, 02Ch, 02Dh, 02Eh, 02Fh
	db 030h, 031h, 032h, 033h, 034h, 035h, 036h, 037h
	db 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh
	db 040h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 05Bh, 05Ch, 05Dh, 05Eh, 05Fh
	db 060h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 07Bh, 07Ch, 07Dh, 07Eh, 07Fh
	db 080h, 081h, 082h, 083h, 084h, 085h, 086h, 087h
	db 088h, 089h, 08Ah, 08Bh, 08Ch, 08Dh, 08Eh, 08Fh
	db 090h, 091h, 092h, 093h, 094h, 095h, 096h, 097h
	db 098h, 099h, 08Ah, 09Bh, 08Ch, 08Dh, 08Eh, 08Fh
	db 0A0h, 0A1h, 0A2h, 0A3h, 0A4h, 0A5h, 0A6h, 0A7h
	db 0A8h, 0A9h, 0AAh, 0ABh, 0ACh, 0ADh, 0AEh, 0AFh
	db 0B0h, 0B1h, 0B2h, 0A3h, 0B4h, 0B5h, 0B6h, 0B7h
	db 0B8h, 0A5h, 0AAh, 0BBh, 0BCh, 0BDh, 0BCh, 0AFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 0DDh, 0DEh, 0DFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0F7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 0DDh, 0DEh, 0FFh

;/////////////////////////////////////////////////////////////////////////////

PUBLIC __tp1251_toupper
__tp1251_toupper:

	db 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h
	db 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh
	db 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h
	db 018h, 019h, 01Ah, 01Bh, 01Ch, 01Dh, 01Eh, 01Fh
	db 020h, 021h, 022h, 023h, 024h, 025h, 026h, 027h
	db 028h, 029h, 02Ah, 02Bh, 02Ch, 02Dh, 02Eh, 02Fh
	db 030h, 031h, 032h, 033h, 034h, 035h, 036h, 037h
	db 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh
	db 040h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 05Bh, 05Ch, 05Dh, 05Eh, 05Fh
	db 060h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 07Bh, 07Ch, 07Dh, 07Eh, 07Fh
	db 080h, 081h, 082h, 081h, 084h, 085h, 086h, 087h
	db 088h, 089h, 08Ah, 08Bh, 08Ch, 08Dh, 08Eh, 08Fh
	db 080h, 091h, 092h, 093h, 094h, 095h, 096h, 097h
	db 098h, 099h, 08Ah, 09Bh, 08Ch, 08Dh, 08Eh, 08Fh
	db 0A0h, 0A1h, 0A1h, 0A3h, 0A4h, 0A5h, 0A6h, 0A7h
	db 0A8h, 0A9h, 0AAh, 0ABh, 0ACh, 0ADh, 0AEh, 0AFh
	db 0B0h, 0B1h, 0B2h, 0B2h, 0A5h, 0B5h, 0B6h, 0B7h
	db 0A8h, 0B9h, 0AAh, 0BBh, 0A3h, 0BDh, 0BDh, 0AFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 0DDh, 0DEh, 0DFh
	db 041h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 049h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 04Fh, 0CFh
	db 0D0h, 0D1h, 0D2h, 055h, 0D4h, 0D5h, 0D6h, 0D7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 045h, 0DEh, 0DFh

;/////////////////////////////////////////////////////////////////////////////

PUBLIC __tp1252_toupper
__tp1252_toupper:

	db 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h
	db 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh
	db 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h
	db 018h, 019h, 01Ah, 01Bh, 01Ch, 01Dh, 01Eh, 01Fh
	db 020h, 021h, 022h, 023h, 024h, 025h, 026h, 027h
	db 028h, 029h, 02Ah, 02Bh, 02Ch, 02Dh, 02Eh, 02Fh
	db 030h, 031h, 032h, 033h, 034h, 035h, 036h, 037h
	db 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh
	db 040h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 05Bh, 05Ch, 05Dh, 05Eh, 05Fh
	db 060h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 07Bh, 07Ch, 07Dh, 07Eh, 07Fh
	db 080h, 081h, 082h, 083h, 084h, 085h, 086h, 087h
	db 088h, 089h, 08Ah, 08Bh, 08Ch, 08Dh, 08Eh, 08Fh
	db 090h, 091h, 092h, 093h, 094h, 095h, 096h, 097h
	db 098h, 099h, 08Ah, 09Bh, 08Ch, 09Dh, 08Eh, 09Fh
	db 0A0h, 0A1h, 0A2h, 0A3h, 0A4h, 0A5h, 0A6h, 0A7h
	db 0A8h, 0A9h, 0AAh, 0ABh, 0ACh, 0ADh, 0AEh, 0AFh
	db 0B0h, 0B1h, 0B2h, 0B3h, 0B4h, 0B5h, 0B6h, 0B7h
	db 0B8h, 0B9h, 0BAh, 0BBh, 0BCh, 0BDh, 0BEh, 0BFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 0DDh, 0DEh, 0DFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0F7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 0DDh, 0DEh, 09Fh

;/////////////////////////////////////////////////////////////////////////////

PUBLIC __tp1253_toupper
__tp1253_toupper:

	db 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h
	db 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh
	db 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h
	db 018h, 019h, 01Ah, 01Bh, 01Ch, 01Dh, 01Eh, 01Fh
	db 020h, 021h, 022h, 023h, 024h, 025h, 026h, 027h
	db 028h, 029h, 02Ah, 02Bh, 02Ch, 02Dh, 02Eh, 02Fh
	db 030h, 031h, 032h, 033h, 034h, 035h, 036h, 037h
	db 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh
	db 040h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 05Bh, 05Ch, 05Dh, 05Eh, 05Fh
	db 060h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 07Bh, 07Ch, 07Dh, 07Eh, 07Fh
	db 080h, 081h, 082h, 083h, 084h, 085h, 086h, 087h
	db 088h, 089h, 08Ah, 08Bh, 08Ch, 08Dh, 08Eh, 08Fh
	db 090h, 091h, 092h, 093h, 094h, 095h, 096h, 097h
	db 098h, 099h, 09Ah, 09Bh, 09Ch, 09Dh, 09Eh, 09Fh
	db 0A0h, 0A1h, 0A2h, 0A3h, 0A4h, 0A5h, 0A6h, 0A7h
	db 0A8h, 0A9h, 0AAh, 0ABh, 0ACh, 0ADh, 0AEh, 0AFh
	db 0B0h, 0B1h, 0B2h, 0B3h, 0B4h, 0B5h, 0B6h, 0B7h
	db 0B8h, 0B9h, 0BAh, 0BBh, 0BCh, 0BDh, 0BEh, 0BFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0A2h, 0B8h, 0B9h, 0BAh
	db 0E0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0F2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0BCh, 0BEh, 0BFh, 0FFh

;/////////////////////////////////////////////////////////////////////////////

PUBLIC __tp1254_toupper
__tp1254_toupper:

	db 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h
	db 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh
	db 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h
	db 018h, 019h, 01Ah, 01Bh, 01Ch, 01Dh, 01Eh, 01Fh
	db 020h, 021h, 022h, 023h, 024h, 025h, 026h, 027h
	db 028h, 029h, 02Ah, 02Bh, 02Ch, 02Dh, 02Eh, 02Fh
	db 030h, 031h, 032h, 033h, 034h, 035h, 036h, 037h
	db 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh
	db 040h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 05Bh, 05Ch, 05Dh, 05Eh, 05Fh
	db 060h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 07Bh, 07Ch, 07Dh, 07Eh, 07Fh
	db 080h, 081h, 082h, 083h, 084h, 085h, 086h, 087h
	db 088h, 089h, 08Ah, 08Bh, 08Ch, 08Dh, 08Eh, 08Fh
	db 090h, 091h, 092h, 093h, 094h, 095h, 096h, 097h
	db 098h, 099h, 08Ah, 09Bh, 08Ch, 09Dh, 09Eh, 09Fh
	db 0A0h, 0A1h, 0A2h, 0A3h, 0A4h, 0A5h, 0A6h, 0A7h
	db 0A8h, 0A9h, 0AAh, 0ABh, 0ACh, 0ADh, 0AEh, 0AFh
	db 0B0h, 0B1h, 0B2h, 0B3h, 0B4h, 0B5h, 0B6h, 0B7h
	db 0B8h, 0B9h, 0BAh, 0BBh, 0BCh, 0BDh, 0BEh, 0BFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 0DDh, 0DEh, 0DFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0F7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 0DDh, 0DEh, 09Fh

;/////////////////////////////////////////////////////////////////////////////

PUBLIC __tp1257_toupper
__tp1257_toupper:

	db 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h
	db 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh
	db 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h
	db 018h, 019h, 01Ah, 01Bh, 01Ch, 01Dh, 01Eh, 01Fh
	db 020h, 021h, 022h, 023h, 024h, 025h, 026h, 027h
	db 028h, 029h, 02Ah, 02Bh, 02Ch, 02Dh, 02Eh, 02Fh
	db 030h, 031h, 032h, 033h, 034h, 035h, 036h, 037h
	db 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh
	db 040h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 05Bh, 05Ch, 05Dh, 05Eh, 05Fh
	db 060h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 07Bh, 07Ch, 07Dh, 07Eh, 07Fh
	db 080h, 081h, 082h, 083h, 084h, 085h, 086h, 087h
	db 088h, 089h, 08Ah, 08Bh, 08Ch, 08Dh, 08Eh, 08Fh
	db 090h, 091h, 092h, 093h, 094h, 095h, 096h, 097h
	db 098h, 099h, 09Ah, 09Bh, 09Ch, 09Dh, 09Eh, 09Fh
	db 0A0h, 0A1h, 0A2h, 0A3h, 0A4h, 0A5h, 0A6h, 0A7h
	db 0A8h, 0A9h, 0AAh, 0ABh, 0ACh, 0ADh, 0AEh, 0AFh
	db 0B0h, 0B1h, 0B2h, 0B3h, 0B4h, 0B5h, 0B6h, 0B7h
	db 0A8h, 0B9h, 0AAh, 0BBh, 0BCh, 0BDh, 0BEh, 0AFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 0DDh, 0DEh, 0DFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0F7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 0DDh, 0DEh, 0FFh

;/////////////////////////////////////////////////////////////////////////////

PUBLIC __tp1258_toupper
__tp1258_toupper:

	db 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h
	db 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh
	db 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h
	db 018h, 019h, 01Ah, 01Bh, 01Ch, 01Dh, 01Eh, 01Fh
	db 020h, 021h, 022h, 023h, 024h, 025h, 026h, 027h
	db 028h, 029h, 02Ah, 02Bh, 02Ch, 02Dh, 02Eh, 02Fh
	db 030h, 031h, 032h, 033h, 034h, 035h, 036h, 037h
	db 038h, 039h, 03Ah, 03Bh, 03Ch, 03Dh, 03Eh, 03Fh
	db 040h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 05Bh, 05Ch, 05Dh, 05Eh, 05Fh
	db 060h, 041h, 042h, 043h, 044h, 045h, 046h, 047h
	db 048h, 049h, 04Ah, 04Bh, 04Ch, 04Dh, 04Eh, 04Fh
	db 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h
	db 058h, 059h, 05Ah, 07Bh, 07Ch, 07Dh, 07Eh, 07Fh
	db 080h, 081h, 082h, 083h, 084h, 085h, 086h, 087h
	db 088h, 089h, 08Ah, 08Bh, 08Ch, 08Dh, 08Eh, 08Fh
	db 090h, 091h, 092h, 093h, 094h, 095h, 096h, 097h
	db 098h, 099h, 09Ah, 09Bh, 08Ch, 09Dh, 09Eh, 09Fh
	db 0A0h, 0A1h, 0A2h, 0A3h, 0A4h, 0A5h, 0A6h, 0A7h
	db 0A8h, 0A9h, 0AAh, 0ABh, 0ACh, 0ADh, 0AEh, 0AFh
	db 0B0h, 0B1h, 0B2h, 0B3h, 0B4h, 0B5h, 0B6h, 0B7h
	db 0B8h, 0B9h, 0BAh, 0BBh, 0BCh, 0BDh, 0BEh, 0BFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0CCh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 0DDh, 0DEh, 0DFh
	db 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h
	db 0C8h, 0C9h, 0CAh, 0CBh, 0ECh, 0CDh, 0CEh, 0CFh
	db 0D0h, 0D1h, 0F2h, 0D3h, 0D4h, 0D5h, 0D6h, 0F7h
	db 0D8h, 0D9h, 0DAh, 0DBh, 0DCh, 0DDh, 0FEh, 09Fh


.code

;/////////////////////////////////////////////////////////////////////////////

I386_ScanMemory PROC pBuffer:DWORD, iSize:DWORD, szScan:DWORD, iLen:DWORD, piLine:DWORD

	push esi
	push edi
	push ebx
	push ebp
	xor eax, eax
	push eax ;LOCAL iLine

	mov esi, [esp+24]
	mov edi, [esp+32]
	mov ecx, [esp+28]
	sub ecx, [esp+36]
	jb quit
	cmp ecx, [esp+28] ; in case iLen is zero
	je quit
	add ecx, 1

	xor ebx, ebx

	mov al, [edi]
nxta:
	cmp al, [esi]
	je save
cont:
	cmp BYTE PTR[esi], 0Ah
	je line
cont2:
	add esi, 1
	sub ecx, 1
	jnz nxta
quit:
	mov eax, -1
	pop ecx
	pop ebp
	pop ebx
	pop edi
	pop esi
	ret
line:
	inc ebx
	jmp cont2
save:
	mov edx, [esp+36]
nxtb:
	sub edx, 1
	jz stop

	mov al, [edi+edx]
	cmp al, [esi+edx]
	je nxtb
rest:
	mov al, [edi]
	jmp cont
stop:
	mov eax, [esp+40]
	mov [eax], ebx
	mov eax, esi
	sub eax, [esp+24]
	pop ecx
	pop ebp
	pop ebx
	pop edi
	pop esi
	ret

I386_ScanMemory ENDP

;/////////////////////////////////////////////////////////////////////////////

I386_ScanMemory_I PROC pBuffer:DWORD, iSize:DWORD, szScan:DWORD, iLen:DWORD, piLine:DWORD, pCPL:DWORD

	push esi
	push edi
	push ebx
	push ebp
	xor eax, eax
	push eax ;LOCAL iLine

	mov esi, [esp+24]
	mov edi, [esp+32]
	mov ecx, [esp+28]
	sub ecx, [esp+36]
	jb quit
	cmp ecx, [esp+28] ; in case iLen is zero
	je quit
	add ecx, 1

	mov ebp, [esp+44]

	xor eax, eax
	xor ebx, ebx

	mov al, [edi]
	mov al, BYTE PTR[ebp+eax]
nxta:
	mov bl, [esi]
	cmp al, BYTE PTR[ebp+ebx]
	je save
cont:
	cmp bl, 0Ah
	je line
cont2:
	add esi, 1
	sub ecx, 1
	jnz nxta
quit:
	mov eax, -1
	pop ecx
	pop ebp
	pop ebx
	pop edi
	pop esi
	ret
line:
	inc DWORD PTR[esp]
	jmp cont2
save:
	mov edx, [esp+36]
nxtb:
	sub edx, 1
	jz stop

	mov al, [edi+edx]
	mov al, BYTE PTR[ebp+eax]
	mov bl, [esi+edx]
	cmp al, BYTE PTR[ebp+ebx]
	je nxtb
rest:
	mov al, [edi]
	mov al, BYTE PTR[ebp+eax]
	jmp cont
stop:
	mov ecx, [esp]
	mov eax, [esp+40]
	mov [eax], ecx
	mov eax, esi
	sub eax, [esp+24]
	pop ecx
	pop ebp
	pop ebx
	pop edi
	pop esi
	ret

I386_ScanMemory_I ENDP

;/////////////////////////////////////////////////////////////////////////////
end
