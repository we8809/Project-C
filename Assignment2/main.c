#include <stdio.h>
#include <string.h>

#include "translate.h"

#define MAX_LENGTH  (10U)

/* TRANSLATE PROGRAM의 실행 방법 -> $ { translate [OPTION] SET1 SET2 } - 허용 인자 개수 = 3, 4개 */

size_t validate_error_wrong_arguments_number_test(int argc, const char* argv[])
{
    /*
        [test의 목적] : 잘못된 인자 갯수가 들어갔을 시 ERROR_CODE_WRONG_ARGUMENTS_NUMBER 반환을 하는 지에 대한 유효성 검사.
        

        [이 TEST를 위해 argument로 들어가야 할 command set]
        1) $ a.exe -i abc            (option 플래그가 들어왔으나 stdout으로 바꿔 줄 character set이 들어오지 않았을 경우
        2) $ a.exe abc               (option 플래그는 없으나 stdout으로  바꿔 줄 character set이 들어오지 않았을 경우
        3) $ a.exe                   (전달 인자가 하나도 들어가지 않았을 경우)
        5) $ a.exe -x def aaa bbb    (전달 인자가 옵션 플래그가 없는 데도 인자가 4개인 경우 - 인자 5개)
        6) $ a.exe -i def aaa aaa    (전달 인자가 정상 옵션 플래그가 있는 경우임에도 character set이 3개 이상인 경우 - 인자 5개)
    */

    size_t error_code = translate(argc, argv);

    return error_code;
}

size_t validate_error_invalid_flag_test(int argc, const char* argv[])
{
    /*
        [test의 목적] : 잘못된 옵션 인자가 들어갔을 시 ERROR_CODE_INVALID_FLAG 반환을 하는 지에 대한 유효성 검사.
        - 이 프로그램은 '-i' 옵션 플래그 만을 지원함.

        [이 TEST를 위해 argument로 들어가야 할 command set]
        1) $ a.exe -i abc def                               (정상 작동)
        2) $ a.exe -[i가 아닌 다른 문자나 문자열] abc def   (플래그 에러 발생)
        3) $ a.exe -x abc def                               (플래그 에러 발생)
        4) $ a.exe abc def aaa                              (옵션 플래그가 들어갈 자리에 임의의 문자열이 와있음 - 인자 4개)
    */
    
    size_t error_code = translate(argc, argv);

    return error_code;
}

size_t validate_error_invalid_format_test(int argc, const char* argv[])
{
    /*
        [test의 목적] : 잘못된 유형의 문자가 들어갔을 시 ERROR_CODE_INVALID_FORMAT 반환을 하는 지에 대한 유효성 검사.
        - 기존 아스키 표에서 출력 가능한 문자들은 모두 올바른 형식의 입력으로 함.
        - 이스케이프 문자의 입력은 다음 문자들 만을 허용.
            • \\
            • \a
            • \b
            • \f
            • \n
            • \r
            • \t
            • \v
            • \'
            • \"

        [이 TEST를 위해 argument로 들어가야 할 command set]
        1) $ a.exe abc\n def                                (정상 작동)
           > 결과 : [abc\n deff]

        2) $ a.exe \abc deff                                (정상 작동)
           > 결과 : [\abc def]

        3) $ a.exe a\"bc defk                               (정상 작동)
           > 결과 : [a\"bc defk]

        4) $ a.exe abc\f defk                               (정상 작동)
           > 결과 : [abc\f defk]

        5) $ a.exe -i \krt def                              (포맷 에러 발생)
           > 결과 : INVALID_FORMAT_ERROR
        
        6) $ a.exe -i aaa b\0b                              (포맷 에러 발생)
           > 결과 : INVALID_FORMAT_ERROR
    */  

    size_t error_code = translate(argc, argv);

    return error_code;
}

size_t validate_duplicate_chr_set_test(int argc, const char* argv[])
{
    /*
        [test의 목적] : 첫 번째 chracter set에서의 중복 문자가 감지 됐을 시, 중복 문자가 올바르게 제거되고 올바른 포맷으로 aligned 되는 지에 대한 검사
        
        1) $ a.exe abaca def                                (정상 작동)
           > 결과 : [bca def]

        2) $ a.exe abaca defgh                              (정상 작동)
           > 결과 : [bca egh]
    */

    size_t error_code = translate(argc, argv);

    return error_code;
}

size_t validate_chr_set_range_test(int argc, const char* argv[])
{
    /*
        [test의 목적] : '-' 범위 지정자 문자를 포함시켰을 때의 올바른 작동을 하는 지에 대한 검사

        1) $ a.exe abaca def                                (정상 작동)
           > 결과 : [bca def]

        2) $ a.exe abaca defgh                              (정상 작동)
           > 결과 : [bca egh]
    */

    size_t error_code = translate(argc, argv);

    return error_code;
}

int main(int argc, const char* argv[])
{
    size_t error_code = 0;
    int cnt = 0;
    int chr = 72;

    
    fprintf(stderr, "[PROGRAM_START]\n");
    fprintf(stderr, "=================================================================================\n");
    fprintf(stderr, "argc : %d\n", argc);
    for (cnt = 0; cnt < argc; cnt++) {
        fprintf(stderr, "argv[%d] : %s\n", cnt, argv[cnt]);
    }
    fprintf(stderr, "=================================================================================\n");
    fprintf(stderr, "\n");
    

    error_code = translate(argc, argv);
    
    /* FINAL : return value check */
    switch (error_code) {
    case ERROR_CODE_WRONG_ARGUMENTS_NUMBER:
        fprintf(stderr, "\nerror : ERROR_CODE_WRONG_ARGUMENTS_NUMBER\n");
        break;
    case ERROR_CODE_INVALID_FLAG:
        fprintf(stderr, "\nerror : ERROR_CODE_INVALID_FLAG\n");
        break;
    case ERROR_CODE_INVALID_FORMAT:
        fprintf(stderr, "\nerror : ERROR_CODE_INVALID_FORMAT\n");
        break;
    case ERROR_CODE_ARGUMENT_TOO_LONG:
        fprintf(stderr, "\nerror : ERROR_CODE_ARGUMENT_TOO_LONG\n");
        break;
    case ERROR_CODE_INVALID_RANGE:
        fprintf(stderr, "\nerror : ERROR_CODE_INVALID_RANGE\n");
        break;
    default:
        fprintf(stderr, "\nSUCCESS\n");
        break;
    }

    return 0;
}
