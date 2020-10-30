#ifndef UTILS_H
#define UTILS_H


static void SerialPrintf(const char* fmt, ...) {
    char buf[100];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    Serial.print(buf);
}

#endif
