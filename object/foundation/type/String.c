//
// Created by kenhuang on 2018/6/7.
//

#include "String.h"
#include "Pointer.h"

private_start(String,Object)
    char* value;
private_end
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object* init(Object* _self,va_list* parms){
    return Class_getSuperClass(StringClazz)->init(_self,parms);
}
static Object* deinit(Object* _self){
    String* self = (String*)_self;
    if(self->private->value){
        free(self->private->value);
    }
    return Class_getSuperClass(StringClazz)->deinit(_self);
}
static char *subString(char *str, int StartPostion, int SubstringLength) {
    int stringlen = 0;
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
    if(Object_isMemberOf(object,StringClazz)){
        String* string = (String*) object;
        return strcmp(self->private->value,string->private->value) == 0;
    }
    return  false;
}
static int hashCode(String*self){
    int a = 0;
    size_t length = strlen(self->private->value);
    if(length > 8){
        length = 8;
    }
    for (int i = 0; i < length ; ++i) {
        a += self->private->value[i]<<(3 * i);
    }
    return a;
}
static int indexOf(char* source, unsigned int sourceOffset,unsigned int sourceCount,
                   char* target, unsigned int targetOffset, unsigned int targetCount,
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
 *  公开方法
 **********************************************************************************************************************/

String* String_new(String* self,char* value){
    if(!self){
        self = Class_newInstance(StringClazz);
    }
    if(value == NULL){
        self->private->value = calloc(1, 1);
        self->private->value[0] = '\0';
    }else{
        size_t allLength = strlen(value) + 1;
        self->private->value = calloc(1, allLength);
        memcpy(self->private->value,value,allLength);
    }

    return self;
}
String* String_newWithOffsetLength(String* self,char* value,int offset,int length){
    if(offset >= 0 && length >= 0 && offset + length <= strlen(value)){
        if(!self){
            self = Class_newInstance(StringClazz);
        }
        if(length == 0 ){
            self->private->value = "";
        }else{
            self->private->value = calloc(1,length);
            memcpy(self->private->value,value + offset,length);
        }
    }
    return self;
}
unsigned int String_length(String*self){
    return strlen(self->private->value);
}
String* String_concat(String*self,String* str){
    unsigned int selfLength = String_length(str);
    unsigned int strLength = String_length(self);
    unsigned int length = selfLength + strLength + 1 ;
    char* buff = malloc(length);
    memcpy(buff,self->private->value,selfLength);
    memcpy(buff + selfLength,str->private->value,strLength + 1);
    String* result = String_new(NULL,buff);
    free(buff);
    return result;
}
char String_charAt(String *self, unsigned int index) {
    assert(index < String_length(self));
    return self->private->value[index];
}
int String_compareTo(String*self,String* str){
    unsigned int selfLength = String_length(str);
    unsigned int strLength = String_length(self);
    unsigned lengthMin = MIN(selfLength,strLength);
    for (int i = 0; i < lengthMin; ++i) {
        if(self->private->value[i] != str->private->value[i]){
            return  self->private->value[i] - str->private->value[i];
        }
    }
    return selfLength - strLength;
}

bool String_startsWith(String *self, String *prefix, unsigned int offset) {
    char* selfValue = self->private->value;
    char* prefixValue = prefix->private->value;
    unsigned int prefixLength = String_length(prefix);
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


unsigned int String_indexOf(String *self, String *str,unsigned int offset){
    return indexOf(self->private->value, 0, String_length(self),
                   str->private->value, 0, String_length(str), offset);
}
String* String_subString(String*self, unsigned int beginIndex, unsigned int endIndex){
    if(endIndex < beginIndex || endIndex > strlen(self->private->value)){
        return  NULL;
    }
    return String_newWithOffsetLength(NULL,self->private->value,beginIndex,endIndex - beginIndex);
}
String* String_toUpperCase(String*self){
   String* str = String_new(NULL,self->private->value);
    char* value = str->private->value;
    for (int i = 0; i < strlen(value); ++i) {
        if(value[i] >= 97 && value[i] <= 122){
            value[i]-= 32;
        }
    }
    return str;
}
String* String_toLowerCase(String*self){
    String* str = String_new(NULL,self->private->value);
    char* value = str->private->value;
    for (int i = 0; i < strlen(value); ++i) {
        if(value[i] >= 65 && value[i] <= 90){
            value[i]+= 32;
        }
    }
    return str;
}
String* String_trim(String*self){
    unsigned int length = String_length(self);
    unsigned int start = 0;
    char* value = self->private->value;
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
        return  self->private->value;
    }
    return "";
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
class_start(String,Object,offsetof(String,private), sizeof(__String))
class_end
static void Class_init(Class1* class){
    ((Class1*)StringClazz)->equals = (bool (*)(Object *, Object *))equals;
    ((Class1*)StringClazz)->hashCode = (int (*)(Object *))hashCode;
}
static void Class_deinit(Class1* class){

}
void StringClazz_fillFieldsMethods(){
        Class_fillDeclaredFields(StringClazz,
                                 "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "value",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "");
        Class_fillDeclaredMethods(StringClazz,
                                  init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  subString,READABLE_UNWRITABLE_PRIVATE,
                                  equals,READABLE_UNWRITABLE_PRIVATE,
                                  hashCode,READABLE_UNWRITABLE_PRIVATE,
                                  String_new,READABLE_UNWRITABLE_PUBLIC,
                                  String_newWithOffsetLength,READABLE_UNWRITABLE_PUBLIC,
                                  String_length,READABLE_UNWRITABLE_PUBLIC,
                                  String_concat,READABLE_UNWRITABLE_PUBLIC,
                                  String_charAt,READABLE_UNWRITABLE_PUBLIC,
                                  String_compareTo,READABLE_UNWRITABLE_PUBLIC,
                                  String_startsWith,READABLE_UNWRITABLE_PUBLIC,
                                  String_endsWith,READABLE_UNWRITABLE_PUBLIC,
                                  String_indexOf,READABLE_UNWRITABLE_PUBLIC,
                                  String_subString,READABLE_UNWRITABLE_PUBLIC,
                                  String_toUpperCase,READABLE_UNWRITABLE_PUBLIC,
                                  String_toLowerCase,READABLE_UNWRITABLE_PUBLIC,
                                  String_trim,READABLE_UNWRITABLE_PUBLIC,
                                  String_getValue,READABLE_UNWRITABLE_PUBLIC,
                                  NULL);
}