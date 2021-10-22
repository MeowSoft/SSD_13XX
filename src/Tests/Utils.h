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

#define lambda(lambda$_ret, lambda$_args, lambda$_body) \
({ \
lambda$_ret lambda$__anon$ lambda$_args \
lambda$_body \
&amp;lambda$__anon$; \
})

#endif
