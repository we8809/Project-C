#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "receipter.h"

void validate_no_item_print_receipt_test(void)
{
    /* 아이템이 없는 데도 receipt가 출력 되는 지에 대한 유효성 검사 */

    time_t time = 1569472997;

    assert(FALSE == print_receipt("receipt0.txt", time));

    printf("validate_no_item_print_receipt_test() -> receipt0.txt\n");
}

void validate_item_count_overflow_test(void)
{
    /* 영수증에 출력되는 item 최대 개수인 10개를 초과하여 추가할 시 유효성 검사 */

    time_t time = 1569472997;

    assert(TRUE == add_item("\a", 10.0));
    assert(TRUE == add_item("2", 10.0));
    assert(TRUE == add_item("3", 10.0));
    assert(TRUE == add_item("4", 10.0));
    assert(TRUE == add_item("5", 10.0));
    assert(TRUE == add_item("6", 10.0));
    assert(TRUE == add_item("7", 10.0));
    assert(TRUE == add_item("8", 10.0));
    assert(TRUE == add_item("9", 10.0));
    assert(TRUE == add_item("10", 10.0));       /* This item length is 25 */

    /* 여기서 FALSE 안뜨면 item overcount */
    assert(FALSE == add_item("11", 2.55));

    set_message("Thank you!\r\n");

    assert(TRUE == print_receipt("receipt0.txt", time));

    printf("validate_item_count_overflow_test() -> receipt0.txt\n");
}

void validate_item_name_length_overflow_test(void)
{
    char buf[2] = { 0, };

    /* 아이템 이름의 max_length(25) 초과 시 최대 길이인 25개의 문자만 출력하는 지에 대해 확인 */
    time_t time = 1569472997;

    assert(TRUE == add_item("\\n\\n\\n\\n\\n\\n\\n\\n\\n\\naabb\\n", 12.45));    /* This item length is 27 */
    /* item 란에 "g>iY8~RTcY|ol$Q?Y}[N`cE\"n" 로 출력되어야 함 */
    assert(TRUE == add_item("hihi", 10.35));

    /* 빈 아이템 이름에 대해서도 출력이 되어야 함 */
    buf[0] = '\0';
    assert(TRUE == add_item(buf, 10.35));
    assert(TRUE == add_item("hellokeys", 10.35));

    assert(TRUE == print_receipt("receipt0.txt", time));

    printf("validate_item_name_length_overflow_test() -> receipt0.txt\n");
}

void validate_message_length_overflow_test(void)
{
    /* 메세지의 max_length(75) 초과 시 올바른 출력 결과를 하는 지에 대해 확인  */
    time_t time = 1569472997;

    assert(TRUE == add_item("Magarita", 12.45));
    set_message("g>iY8~RTcY|ol$Q?Y}[N`cE\"ng>iY8~RTcY|ol$Q?Y}[N`cE\"ng>iY8~RTcY|ol$Q?Y}[N`cE\"ng>iY8~RTcY|ol$Q?Y}[N`cE\"nab");
    /*
        위 메세지는 
        
        g>iY8~RTcY|ol$Q?Y}[N`cE"ng>iY8~RTcY|ol$Q?Y}[N`cE"n
        g>iY8~RTcY|ol$Q?Y}[N`cE"n
    
        로 출력되어야 함.
    */

    set_message("Thank You!");

    assert(TRUE == print_receipt("receipt0.txt", time));

    printf("validate_message_length_overflow_test() -> receipt0.txt\n\n");

    /* receipt1.txt */
    assert(TRUE == add_item("Magarita", 12.45));
    set_message("SPq!K5F,gjEo&!m89MWC)0F+U_G-Fj` TcHA~$ttahg;#&)w&]-|'Vn(-:x*XZi%JHUL_|J5Q\"%");
             /*  SPq!K5F,gjEo&!m89MWC)0F+U_G-Fj` TcHA~$ttahg;#&)w&]
                 -|'Vn(-:x*XZi%JHUL_|J5Q\"% 
             */

    set_message("hi");

    assert(TRUE == print_receipt("receipt1.txt", time));

    printf("validate_message_length_overflow_test() -> receipt1.txt\n\n");

    /* receipt2.txt */
    assert(TRUE == add_item("Magarita", 12.45));
    set_message("\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n");
    set_message("\\n");
    /* \\n\\n\\n\\n\\n \\n\\n\\n\\n\\n \\n\\n\\n\\n\\n \\n\\n\\n\\n\\n \\n\\n\\n\\n\\n \\n\\n\\n\\n\\n \\n\\n\\n\\n\\n \\n\\na */

    assert(TRUE == print_receipt("receipt2.txt", time));

    printf("validate_message_length_overflow_test() -> receipt2.txt\n\n");
}

void validate_tip_test(void)
{
    /* 1. tip 갱신 여부 검사, 2. 다음 영수증 출력 시 tip 초기화 여부 검사 */

    time_t time = 1569472997;

    assert(TRUE == add_item("Magarita", 12.45));

    set_tip(10.24);
    set_tip(11.34);     /* Tip : 11.34 */

    assert(TRUE == print_receipt("receipt0.txt", time));

    printf("tip is setted. Let's check it! -> receipt0.txt\n");

    assert(TRUE == add_item("seunghwan", 900.12));

    assert(TRUE == print_receipt("receipt1.txt", time));

    printf("tip is initialized. Let's check it! -> receipt1.txt\n");
}

void validate_message_test(void)
{
    char buf[2] = { 0, };

    /* 1. message 출력 여부 검사. 2. 다음 영수증 출력 시 message를 설정하지 않았을 시 잘 초기화 되는 지 여부 검사 */
    time_t time = 1569472997;

    /* receipt0.txt */
    assert(TRUE == add_item("Magarita", 12.45));

    assert(TRUE == print_receipt("receipt0.txt", time));

    printf("validate_message_test() -> receipt0.txt\n\n");

    /* receipt1.txt */
    assert(TRUE == add_item("seunghwan", 900.12));
    set_message("&WQ*XCCD^@kmhUc[8Y`=,<\\IJ3=*?Nj]j)!w;zP`P94u'H1vH]\\nuad");      /* strlen = 58 */

    assert(TRUE == print_receipt("receipt1.txt", time));

    printf("validate_message_test() -> receipt1.txt\n\n");

    /* receipt2.txt */
    assert(TRUE == add_item("donghyun", 900.12));
    assert(TRUE == add_item("donghyun", 900.12));
    assert(TRUE == add_item("donghyun", 900.12));
    assert(TRUE == add_item("donghyun", 900.12));
    assert(TRUE == add_item("donghyun", 900.12));
    assert(TRUE == add_item("donghyun", 900.12));
    assert(TRUE == add_item("donghyun", 900.12));
    assert(TRUE == add_item("donghyun", 900.12));
    assert(TRUE == add_item("donghyun", 900.12));
    assert(TRUE == add_item("donghyun", 900.12));
  
    set_message("<Aihw}+}5s}Q:O=qVCAW',[uNq1(JSUD@e|JS.Em+W_NGLIoKD");
    /* \n */
    set_message("KP12345123451234512345123a");

    assert(TRUE == print_receipt("receipt2.txt", time));

    printf("validate_message_test() -> receipt2.txt\n\n");

    /* receipt3.txt */
    assert(TRUE == add_item("kn '1A\\", 900.12));

    set_message("\\nabl");
    /* \n\n\n\n\n \n\n\n\n\n \n\n\n\n\n \n\n\n\n\n \n\n\n\n\n \n\n\n\n\n \n\n\n\n\n  / \n\n\ */

    assert(TRUE == print_receipt("receipt3.txt", time));

    printf("validate_message_test() -> receipt3.txt\n\n");

    /* receipt4.txt */
    buf[0] = '\0';

    /* + 빈 메세지에 대해서도 출력이 되야 함 */
    assert(TRUE == add_item(buf, 1.2));
    assert(TRUE == add_item("seung", 900.12));
    assert(TRUE == add_item("hwan", 100.13));

    set_message(buf);
    set_message("\\n");

    assert(TRUE == print_receipt("receipt4.txt", time));

    printf("validate_message_test() -> receipt4.txt\n\n");

    /* receipt5.txt */
    assert(TRUE == add_item(";];l.b4(%bb:}a'[A0d}=h<\"aa", 900.12));

    set_message("<Aihw}+}5s}Q:O=qVCAW',[uNq1(JSUD@e|JS.Em+W_NGLIoKDKP");

    assert(TRUE == print_receipt("receipt5.txt", time));

    printf("validate_message_test() -> receipt5.txt\n\n");

    /* receipt6.txt */
    assert(TRUE == add_item("4^q7OS0Y2", 900.12));
    assert(TRUE == add_item("P{/PZ#_F~HAVI''M(\\!N~x[S", 100.13));
    assert(TRUE == add_item("gx4?oMt^UQ", 100.13));
    assert(TRUE == add_item("?&`-", 100.13));
    assert(TRUE == add_item(")^Cv", 100.13));
    assert(TRUE == add_item("XInoF4`~;3b", 100.13));
    assert(TRUE == add_item("/Td", 100.13));
    assert(TRUE == add_item("d0d`W^z^2{=n,", 93.0));

    assert(TRUE == print_receipt("receipt6.txt", time));

    printf("validate_message_test() -> receipt6.txt\n\n");
}

void validate_addItems_tip_message_print_test(void)
{
    /* add_items(), add_tip(), add_message(), print_receipt()의 조합 테스트 */
    time_t time = 1569472997;

    /* receipt0.txt */
    set_message("hihi");
    assert(TRUE == add_item("Magarita", 12.45));

    assert(TRUE == print_receipt("receipt0.txt", time));        /* message가 출력되면 안 됨 */

    printf("validate_addItems_tip_message_print_test -> receipt0.txt\n");

    /* receipt1.txt */
    set_tip(10.22);
    set_message("hihi");

    assert(TRUE == add_item("Magarita", 12.45));

    assert(TRUE == print_receipt("receipt1.txt", time));        /* message와 tip 모두 출력되면 안 됨 */
    
    printf("validate_addItems_tip_message_print_test -> receipt1.txt\n");

    /* receipt2.txt */
    assert(TRUE == add_item("Magarita", 12.45));

    set_tip(10.22);
    set_message("hihi");

    assert(TRUE == print_receipt("receipt2.txt", time));        /* message와 tip 모두 출력 */

    printf("validate_addItems_tip_message_print_test -> receipt2.txt\n");
}

void validate_item_discharge_for_zero_test(void)
{
    /* 서비스 음식(가격이 0원인 음식)에 대해 영수증의 아이템 목록으로 추가할 수 있는 지 여부 검사 */

    time_t time = 1569472997;

    /* receipt0.txt */
    assert(TRUE == add_item("Magarita", 0.0));          
    assert(TRUE == add_item("TIl9Qj%7vE@=)21bW)03Z", 522.40));

    set_message("^QYNntnn3UF*Qvi~@oR\\d?/f>!,f3OcI)O*ytOova'UTuk0#ODs-b}b{U)|;Nwe");

    assert(TRUE == print_receipt("receipt0.txt", time)); 

    printf("validate_item_discharge_for_zero_test -> receipt0.txt\n");

    /* receipt1.txt */
    assert(TRUE == add_item("Magarita", 0.0));
    assert(TRUE == add_item("TIl9Qj%7vE@=)21bW)03Z", 522.40));

    set_message("^QYNntnn3UF*Qvi~@oR\\d?/f>!,f3OcI)O*ytOova'UTuk0#OD");
    set_message("s-b}b{U)|;Nwe");

    assert(TRUE == print_receipt("receipt1.txt", time));

    printf("validate_item_discharge_for_zero_test -> receipt1.txt\n");
}

int main(void)
{
    validate_message_test();
   
    return 0;
}
