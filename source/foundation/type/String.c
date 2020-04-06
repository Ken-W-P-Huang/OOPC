//
// Created by kenhuang on 2019-10-21.
//

#include "String.h"
#include "../interface/Comparable.h"

PRIVATE(String)
    char* value;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(String *self) {
    free(self->p->value);
}

static char *subString(char *str, int StartPostion, int SubstringLength) {
    size_t stringlen = 0;
    int i = 0;
    int x = 0;
    char *tmp;
    stringlen = strlen(str);
    tmp = (char *)malloc(sizeof(char)*(SubstringLength + 1));
    if ((StartPostion < 0) || (SubstringLength <= 0) || (stringlen == 0) || (StartPostion >= stringlen)) {
        strcpy(tmp, "\0");
        return tmp;
    }
    for (i = StartPostion; ((i < stringlen) && (x < SubstringLength)); i++) {
        tmp[x] = str[i];
        x++;
    }
    tmp[x] = '\0';
    return tmp;
}

static bool equals(String*self,Object* object){
    if((Object*)self == object){
        return  true;
    }
    if(isMemberOf(object, (ClassType*)StringClass)){
        String* string = (String*) object;
        return strcmp(self->p->value,string->p->value) == 0;
    }
    return  false;
}

static int hashCode(String*self){
    int a = 0;
    size_t length = strlen(self->p->value);
    if(length > 8){
        length = 8;
    }
    for (int i = 0; i < length ; ++i) {
        a += self->p->value[i]<<(3 * i);
    }
    return a;
}

static int indexOf(const char* source, unsigned int sourceOffset,unsigned int sourceCount,
                   const char* target, unsigned int targetOffset, unsigned int targetCount,
                   unsigned int fromIndex){
    if (fromIndex >= sourceCount) {
        return (targetCount == 0 ? sourceCount : -1);
    }
    if (targetCount == 0) {
        return fromIndex;
    }
    char first = target[targetOffset];
    int max = sourceOffset + (sourceCount - targetCount);
    for (int i = sourceOffset + fromIndex; i <= max; i++) {
        if (source[i] != first) {
            while (++i <= max && source[i] != first);
        }
        if (i <= max) {
            int j = i + 1;
            int end = j + targetCount - 1;
            for (int k = targetOffset + 1; j < end && source[j]
                                                      == target[k]; j++, k++);
            if (j == end) {
                return i - sourceOffset;
            }
        }
    }
    return -1;
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
String *String_init(String *self,char* value) {
    if (Object_init((Object*)self)) {
        if(value == NULL){
            self->p->value = calloc(1, 1);
            self->p->value[0] = '\0';
        }else{
            size_t allLength = strlen(value) + 1;
            self->p->value = calloc(1, allLength);
            memcpy(self->p->value,value,allLength);
        }
    }
    return self;
}
//todo
String* String_initWithOffsetLength(String* self,char* value,int offset,size_t length){
    if(offset >= 0 && length >= 0 && offset + length <= strlen(value)){
        if(length == 0 ){
            self->p->value = "";
        }else{
            self->p->value = calloc(1,length);
            memcpy(self->p->value,value + offset,length);
        }
    }
    return self;
}

size_t String_length(String*self){
    return strlen(self->p->value);
}

String* String_concat(String*self,String* str){
    size_t selfLength = String_length(str);
    size_t strLength = String_length(self);
    size_t length = selfLength + strLength + 1 ;
    char* buff = malloc(length);
    memcpy(buff,self->p->value,selfLength);
    memcpy(buff + selfLength,str->p->value,strLength + 1);
    String* result = String_init(alloc((ClassType*)StringClass),buff);
    free(buff);
    return result;
}

char String_charAt(String *self, unsigned int index) {
    assert(index < String_length(self));
    return self->p->value[index];
}

int String_compareTo(String*self,String* str){
    size_t selfLength = String_length(self);
    size_t strLength = String_length(str);
    size_t lengthMin = MIN(selfLength,strLength);
    for (int i = 0; i < lengthMin; ++i) {
        if(self->p->value[i] != str->p->value[i]){
            return  self->p->value[i] - str->p->value[i];
        }
    }
    return (int)(selfLength - strLength);
}

bool String_startsWith(String *self, String *prefix, size_t offset) {
    char* selfValue = self->p->value;
    char* prefixValue = prefix->p->value;
    size_t prefixLength = String_length(prefix);
    if(offset > String_length(self) - prefixLength ){
        return  false;
    }
    for (int i = 0; i < prefixLength ; ++i) {
        if(selfValue[offset + i] != prefixValue[i]){
            return  false;
        }
    }
    return true;
}

bool  String_endsWith(String*self,String*suffix){
    return String_startsWith(self, suffix, String_length(self) -  String_length(suffix));
}


int String_indexOf(String *self, String *str,unsigned int offset){
    return indexOf(self->p->value, 0, (unsigned int)String_length(self),
                   str->p->value, 0, (unsigned int)String_length(str), offset);
}

String* String_subString(String*self, unsigned int beginIndex, size_t endIndex){
    if(endIndex < beginIndex || endIndex > strlen(self->p->value)){
        return  NULL;
    }
    return String_initWithOffsetLength(alloc((ClassType*)StringClass),self->p->value,beginIndex,endIndex - beginIndex);
}

String* String_toUpperCase(String*self){
    String* str = String_init(alloc((ClassType*)StringClass),self->p->value);
    char* value = str->p->value;
    for (int i = 0; i < strlen(value); ++i) {
        if(value[i] >= 97 && value[i] <= 122){
            value[i]-= 32;
        }
    }
    return str;
}

String* String_toLowerCase(String*self){
    String* str = String_init(alloc((ClassType*)StringClass),self->p->value);
    char* value = str->p->value;
    for (int i = 0; i < strlen(value); ++i) {
        if(value[i] >= 65 && value[i] <= 90){
            value[i]+= 32;
        }
    }
    return str;
}

String* String_trim(String*self){
    size_t length = String_length(self);
    unsigned int start = 0;
    char* value = self->p->value;
    while((start < length) && value[start] <= ' '){
        start++;
    }
    while (start <length && value[length - 1] <= ' '){
        length--;
    }
    return String_subString(self,start,length);
}

char* String_getValue(String*self){
    if(self){
        return  self->p->value;
    }
    return "";
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
static void clinit();
static StringClassType StringClazz[] = {
        {
                NULL,
                "String",
                sizeof(String),
                sizeof(String_P),
                offsetof(String, p),
                sizeof(StringClassType),
#if defined(__CLASS_ATTRIBUTE__)
                REAL,
                false,
#endif
                clinit,
                (void (*)(Object *)) deinit,
                NULL,
#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)
                /*需要提前初始化重写，供hashmap使用*/
                (int (*)(Object*))hashCode,
                (bool (*)(Object*,Object*))equals,
#endif
        }
};
StringClassType *const StringClass = StringClazz;
static void clinit() {
    Class_fillDeclaredFields((ClassType*) StringClass,
                             "value", PointerClass, AccessClass->READABLE_WRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType*) StringClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              subString, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              equals, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              hashCode, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              String_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_initWithOffsetLength, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_length, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_concat, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_charAt, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_compareTo, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_startsWith, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_endsWith, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_indexOf, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_subString, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_toUpperCase, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_toLowerCase, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_trim, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              String_getValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
    Class_addFunctions((ClassType*) StringClass, String_init,
                       String_getValue,
                       String_initWithOffsetLength,
                       String_trim,
                       String_toLowerCase,
                       String_toUpperCase,
                       String_subString,
                       String_indexOf,
                       String_endsWith,
                       String_startsWith,
                       String_compareTo,
                       String_charAt,
                       String_concat,
                       String_length, NULL);
    implements((ClassType*)StringClass,(Object*)&(StringClass->Comparable),(ClassType*)ComparableClass,String_compareTo,
            NULL);
END