#include <Arduino.h>
#include <string.h>
#include "utf8To.h"   // header-ul folosit pentru declararea funcției utf8To()

// Conversie UTF-8 → coduri pentru GLCD
// suport simplu pentru diacritice românești
char* utf8To(const char* txt, bool uppercase) {
    static char buffer[512]; // buffer static pentru text
    size_t i = 0;
    size_t j = 0;

    while (txt[i] && j < sizeof(buffer) - 1) {
        unsigned char c = txt[i];

        // diacritice românești UTF-8 (2-byte)
        if ((unsigned char)txt[i] == 0xC3) { // început diacritică mare sau mică
            unsigned char next = txt[i + 1];
            switch (next) {
                case 0x82: buffer[j++] = uppercase ? 'Ă' : 'ă'; break; // Ă / ă
                case 0xA2: buffer[j++] = uppercase ? 'Â' : 'â'; break; // Â / â
                case 0x8E: buffer[j++] = uppercase ? 'Ș' : 'ș'; break; // Ș / ș
                case 0x98: buffer[j++] = uppercase ? 'Ț' : 'ț'; break; // Ț / ț
                case 0xAE: buffer[j++] = uppercase ? 'Î' : 'î'; break; // Î / î
                default: buffer[j++] = '?'; break; // orice altă diacritică necunoscută
            }
            i += 2;
        } else {
            // litere ASCII simple
            char ch = txt[i++];
            if (uppercase && ch >= 'a' && ch <= 'z') ch -= 32;
            buffer[j++] = ch;
        }
    }

    buffer[j] = 0; // terminator
    return buffer;
}