#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>//
#include "NativeJava.h"
#include "dablooms.h"

#define CAPACITY 2000000
#define ERROR_RATE 0.0005

int get_file_lines_number(char *filename) {
    int lines;
    FILE *fp;
    char ch;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while(!feof(fp)) {
      ch = fgetc(fp);
      if(ch == '\n') {
        lines++;
      }
    }

    return lines;
}

static void chomp_line(char *word) {
    char *p;
    if ((p = strchr(word, '\r'))) {
        *p = '\0';
    }
    if ((p = strchr(word, '\n'))) {
        *p = '\0';
    }
}

int get_main_domain(char *domain, char *main_domain) {
    char tld[20] = {0}; //Top-level domain
    char domain_except_cc_tld[100] = {0}; // except Country code top-level domain
    char domain_except_all_tlds[100] = {0};// except all top-level domain

    int len_domain = strlen(domain);
    if (len_domain >= 99) {
        return 0;
    }

    //get cc_tld
    strncpy(domain_except_cc_tld, domain, sizeof(domain_except_cc_tld) -1);
    domain_except_cc_tld[sizeof(domain_except_cc_tld) - 1] = '\0';

    char cc_tld[10] = {0};
    char cc_tlds[] = ".ac .ad .ae .af .ag"
        ".ai .al .am .an .ao .aq .ar .as .at"
        ".au .aw .az .ba .bb .bd .be .bf .bg"
        ".bh .bi .bj .bm .bn .bo .br .bs .bt"
        ".bv .bw .by .bz .ca .cc .cd .cf .cg"
        ".ch .ci .ck .cl .cm .cn .co .cr .cu"
        ".cv .cx .cy .cz .de .dj .dk .dm .do"
        ".dz .ec .ee .eg .eh .er .es .et .eu"
        ".fi .fj .fk .fm .fo .fr .ga .gd .ge"
        ".gf .gg .gh .gi .gl .gm .gn .gp .gq"
        ".gr .gs .gt .gu .gw .gy .hk .hm .hn"
        ".hr .ht .hu .id .ie .il .im .in .io"
        ".iq .ir .is .it .je .jm .jo .jp .ke"
        ".kg .kh .ki .km .kn .kp .kr .kw .ky"
        ".kz .la .lb .lc .li .lk .lr .ls .lu"
        ".ma .mc .md .me .mg .mh .mk .ml .mm"
        ".mn .mo .mp .mq .mr .ms .mt .mu .mv"
        ".mw .mx .my .mz .na .nc .ne .nf .ng"
        ".ni .nl .no .np .nr .nu .nz .om .pa"
        ".pe .pf .pg .ph .pk .pl .pm .pn .pr"
        ".ps .pt .pw .py .qa .re .ro .ru .rw"
        ".sa .sb .sc .sd .se .sg .sh .si .sj"
        ".sk .sm .sn .so .sr .st .sv .sy .sz"
        ".tc .td .tf .tg .th .tj .tk .tl .tm"
        ".tn .to .tp .tr .tt .tv .tw .tz .ua"
        ".ug .uk .um .us .uy .uz .va .vc .ve"
        ".vg .vi .vn .vu .wf .ws .ye .yt .yu"
        ".yr .za .zm .zw .home .lan .club .local"
        ".intern .belkin";

    char *temp_cc_tld = strrchr(domain, '.');

    if (NULL == temp_cc_tld) {
        return 0;
    }

    int len_cc_tld = strlen(temp_cc_tld);
    if (len_cc_tld >= 9) {
        return 0;
    }

    char *find_cc_tld;
    find_cc_tld = strstr(cc_tlds, temp_cc_tld);

    if (NULL != find_cc_tld) {
        strncpy(cc_tld, temp_cc_tld, sizeof(cc_tld)-1);
        cc_tld[sizeof(cc_tld) - 1] = '\0';

        strcat(tld, cc_tld);
        int len_intercept_cc = temp_cc_tld - domain;
        strncpy(domain_except_cc_tld, domain, len_intercept_cc);
        domain_except_cc_tld[len_intercept_cc] = '\0';   
    }

    //get g_tld and name
    strncpy(domain_except_all_tlds, domain_except_cc_tld, sizeof(domain_except_all_tlds) -1);
    domain_except_all_tlds[sizeof(domain_except_all_tlds) - 1] = '\0';

    char g_tld[10] = {0};
    char g_tlds[] = ".com  .net  .org  .gov  .mil  .edu  .biz  .info  .pro  .name  .coop  .travel"
    ".xxx  .idv  .aero  .museum  .mobi  .asia  .tel  .int  .post  .jobs  .cat  .co"
    ".cc  .xyz  .top  .su  .lt  .link  .corp  .stream  .site  .email  .online  .win"
    ".ly  .men  .media  .bid  .news  .lv  .rs  .space  .sx  .bid  .ovh  .gdn  .ac"
    ".tech  .dev  .guru  .click  .life  .live  .one  .video  .host  .world  .press"
    ".tokyo  .website  .download  .today  .rocks  .red";

    char *temp_g_tld = strrchr(domain_except_cc_tld, '.');

    if (NULL == temp_g_tld) {
        strncpy(main_domain, domain_except_cc_tld, sizeof(domain_except_cc_tld));
        strcat(main_domain, tld);
        return 1;
    }


    int len_g_tld = strlen(temp_g_tld);
    if (len_g_tld >= 9) {
        return 0;
    }
    
    char *find_g_tld;
    find_g_tld = strstr(g_tlds, temp_g_tld);
    
    if (NULL != find_g_tld) {
        strncpy(g_tld, temp_g_tld, sizeof(g_tld)-1);
        g_tld[sizeof(g_tld) - 1] = '\0';

        strncpy(tld, g_tld, sizeof(tld));
        strcat(tld, cc_tld); 

        int len_intercept_g = temp_g_tld - domain_except_cc_tld;
        strncpy(domain_except_all_tlds, domain, len_intercept_g);
        domain_except_all_tlds[len_intercept_g] = '\0';
    }

    if (tld[0] == '\0') {
        return 0;
    }

    char *find_main_domain = strrchr(domain_except_all_tlds, '.');
    if (NULL == find_main_domain) {
        strncpy(main_domain, domain_except_all_tlds, sizeof(main_domain) - 1);
        main_domain[sizeof(main_domain) - 1] = '\0';
    }

    else{
        strncpy(main_domain, find_main_domain, sizeof(main_domain) - 1);
        main_domain[sizeof(main_domain) - 1] = '\0';
    }

    if (main_domain[0] == '.') {
        memmove(main_domain, main_domain+1, strlen(main_domain));
    }

    strcat(main_domain, tld); 

    return 1;
}

int find_in_white_list(char * domain, const char *white_list_file, const char *bloom_file) {
    if (domain == NULL) {
        return 0;
    }

    char main_domain[100] = {0};

    int state = get_main_domain(domain, main_domain);

    if (state == 0) {
        return 0;
    }

    FILE *fp;
    char word[256];
    counting_bloom_t *bloom;

    bloom = new_counting_bloom(CAPACITY, ERROR_RATE, bloom_file);
    int i;

    fp = fopen(white_list_file, "r");

    for (i = 0; fgets(word, sizeof(word), fp) && (i < CAPACITY); i++) {
        chomp_line(word);
        counting_bloom_add(bloom, word, strlen(word));
    }

    int match;
    match = counting_bloom_check(bloom, main_domain, strlen(main_domain));
    
    free_counting_bloom(bloom);
    fclose(fp);

    return match;
}

JNIEXPORT jint JNICALL Java_NativeJava_match_1white_1dns(JNIEnv *env, jobject obj, jstring white_dns){
  char white_file[] = "./all_white.txt";
  char bloom_file[] = "./bloom.txt";
  char unmatch_file[] = "./all_new_visualization.txt";

  //int line = get_file_lines_number(white_file);

  printf("hello world!\n");
  const jbyte *str = (const jbyte *)env->GetStringUTFChars( white_dns, JNI_FALSE );
  int match = find_in_white_list((char *)str, white_file, bloom_file);
  
  env->ReleaseStringUTFChars( white_dns, (const char *)str );
  printf("%d\n", match);
  return match; 
}

