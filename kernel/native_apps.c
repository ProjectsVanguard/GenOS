#include "native_apps.h"
#define KEY_UP 256u
#define KEY_DOWN 257u
#define KEY_ENTER 258u
#define KEY_ESCAPE 259u
#define KEY_BACKSPACE 260u

static const char *names[] = {"NOTEPAD (RAM ONLY)", "CALCULATOR", "SYSTEM INFO", "NATIVE TERMINAL"};
static const char *descriptions[] = {
    "TYPE TO WRITE. CONTENT IS VOLATILE.",
    "TYPE INTEGER EXPRESSIONS: 12+3 THEN ENTER.",
    "THIS APP IS COMPILED INTO GENKERNEL.",
    "TYPE HELP, CLEAR OR ABOUT."
};
static void label(const genos_native_ui *u, int x, int y, const char *s, uint32_t c) {
    u->text(x, y, s, c, 2);
}
static void panel(const genos_native_ui *u, const char *title) {
    uint32_t bg=u->rgb(19,27,44), edge=u->rgb(91,126,202), fg=u->rgb(240,244,255);
    u->rect(0,0,(int)u->width,(int)u->height,u->rgb(12,18,31));
    u->rect(24,24,(int)u->width-48,(int)u->height-48,bg);
    u->rect(24,24,(int)u->width-48,4,edge);
    label(u,44,42,title,fg);
}
void genos_native_apps_render(const genos_native_ui *u, unsigned selected, unsigned screen,
                              const char *note, unsigned note_len,
                              const char *calc, const char *result) {
    uint32_t fg=u->rgb(235,241,255), muted=u->rgb(160,178,205), accent=u->rgb(102,220,198);
    if (!u || !u->rgb || !u->rect || !u->text) return;
    if (screen==0) {
        panel(u,"GENOS NATIVE APPLICATIONS");
        label(u,48,88,"BUILT INTO THE KERNEL IMAGE",accent);
        for (unsigned i=0;i<4;i++) {
            int y=132+(int)i*54;
            u->rect(44,y,(int)u->width-88,42,i==selected?u->rgb(44,65,101):u->rgb(25,36,56));
            label(u,58,y+11,names[i],i==selected?fg:muted);
        }
        u->text(48,(int)u->height-74,"UP/DOWN SELECT - ENTER OPEN",muted,1);
        u->text(48,(int)u->height-54,"ESC RETURNS TO THIS MENU",muted,1);
    } else {
        panel(u, screen==1?"NATIVE NOTEPAD":screen==2?"NATIVE CALCULATOR":screen==3?"GENOS SYSTEM INFO":"NATIVE TERMINAL");
        if (screen==1) {
            u->text(48,94,"RAM BUFFER ONLY - NOT SAVED TO DISK",accent,1);
            for (unsigned i=0;i<note_len;i+=70) {
                char line[71]; unsigned n=note_len-i; if(n>70)n=70;
                for(unsigned j=0;j<n;j++) { line[j]=note[i+j]; }\n                line[n]=0;
                u->text(48,124+(int)(i/70)*18,line,fg,1);
                if(i>=490) break;
            }
            u->text(48,(int)u->height-70,"TYPE TO WRITE - BACKSPACE DELETE - ESC MENU",muted,1);
        } else if (screen==2) {
            u->text(48,112,"INTEGER + - * / EXPRESSIONS",muted,1);
            label(u,48,150,calc[0]?calc:"TYPE E.G. 12+3",fg);
            label(u,48,208,result,accent);
            u->text(48,(int)u->height-70,"ENTER CALCULATE - BACKSPACE EDIT - ESC MENU",muted,1);
        } else if (screen==3) {
            u->text(48,112,"GENKERNEL FREESTANDING X86-64",accent,1);
            u->text(48,142,"NATIVE C APP DISPATCH IS ACTIVE",fg,1);
            u->text(48,172,"NO USER MODE OR FILESYSTEM YET",muted,1);
            u->text(48,202,"WEB .GEM APPS ARE NOT RUN NATIVELY",muted,1);
        } else {
            u->text(48,112,"BUILT-IN COMMANDS: HELP, ABOUT, CLEAR",accent,1);
            u->text(48,148,"TYPE COMMAND AND PRESS ENTER",muted,1);
            label(u,48,190,calc,fg);
            label(u,48,236,result,accent);
        }
    }
    (void)descriptions;
}
static long parse_number(const char *s, unsigned *p) {
    long n=0; while(s[*p]>='0'&&s[*p]<='9'){ if(n<1000000000L)n=n*10+(s[*p]-'0'); (*p)++; } return n;
}
static void calculate(const char *s, char *out) {
    unsigned p=0; long a=parse_number(s,&p); char op=s[p++]; long b=parse_number(s,&p); long v=0;
    if (p==0 || op==0 || s[p]!=0) { out[0]='I';out[1]='N';out[2]='V';out[3]='A';out[4]='L';out[5]='I';out[6]='D';out[7]=0; return; }
    if(op=='+')v=a+b; else if(op=='-')v=a-b; else if(op=='*')v=a*b;
    else if(op=='/'&&b!=0)v=a/b; else { const char *e="DIV BY ZERO/OP"; unsigned i=0;while(e[i]){out[i]=e[i];i++;}out[i]=0;return; }
    char tmp[24]; unsigned n=0,i=0; int neg=v<0; unsigned long x=neg?(unsigned long)(-(v+1))+1UL:(unsigned long)v;
    do{tmp[n++]=(char)('0'+x%10);x/=10;}while(x&&n<22); if(neg)out[i++]='-'; while(n)out[i++]=tmp[--n];out[i]=0;
}
unsigned genos_native_apps_key(unsigned key, unsigned selected, unsigned screen,
                               char *note, unsigned *note_len, char *calc, unsigned *calc_len,
                               char *result) {
    if(screen==0){ if(key==KEY_UP)selected=(selected+3)%4; else if(key==KEY_DOWN)selected=(selected+1)%4; else if(key==KEY_ENTER)screen=selected+1; }
    else if(key==KEY_ESCAPE)screen=0;
    else if(screen==1){
        if(key==KEY_BACKSPACE){if(*note_len)note[--*note_len]=0;}
        else if(key>=32&&key<127&&*note_len<560){note[(*note_len)++]=(char)key;note[*note_len]=0;}
        else if(key==KEY_ENTER&&*note_len<560){note[(*note_len)++]=' ';note[*note_len]=0;}
    } else if(screen==2){
        if(key==KEY_BACKSPACE){if(*calc_len)calc[--*calc_len]=0;}
        else if(key==KEY_ENTER){calculate(calc,result);}
        else if(((key>='0'&&key<='9')||key=='+'||key=='-'||key=='*'||key=='/')&&*calc_len<22){calc[(*calc_len)++]=(char)key;calc[*calc_len]=0;}
    } else if(screen==4){
        if(key==KEY_BACKSPACE){if(*calc_len)calc[--*calc_len]=0;}
        else if(key==KEY_ENTER){ if(calc[0]=='H'&&calc[1]=='E'&&calc[2]=='L'&&calc[3]=='P'){const char*s="HELP ABOUT CLEAR";unsigned i=0;while(s[i]){result[i]=s[i];i++;}result[i]=0;} else if(calc[0]=='A'){const char*s="GENOS NATIVE SHELL";unsigned i=0;while(s[i]){result[i]=s[i];i++;}result[i]=0;} else if(calc[0]=='C'){result[0]=0;} else {const char*s="UNKNOWN - TYPE HELP";unsigned i=0;while(s[i]){result[i]=s[i];i++;}result[i]=0;} *calc_len=0;calc[0]=0;}
        else if(key>=32&&key<127&&*calc_len<22){char c=(char)key;if(c>='a'&&c<='z')c=(char)(c-'a'+'A');calc[(*calc_len)++]=c;calc[*calc_len]=0;}
    }
    return (screen<<8)|(selected&255u);
}
