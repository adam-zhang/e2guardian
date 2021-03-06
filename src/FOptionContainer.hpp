// FOptionContainer class - contains the options for a filter group,
// including the banned/grey/exception site lists and the content/site/url regexp lists

// For all support, instructions and copyright go to:
// http://e2guardian.org/
// Released under the GPL v2, with the OpenSSL exception described in the README file.

#ifndef __HPP_FOPTIONCONTAINER
#define __HPP_FOPTIONCONTAINER

// INCLUDES

#include "String.hpp"
#include "HTMLTemplate.hpp"
#include "ListContainer.hpp"
#include "ListMeta.hpp"
#include "LanguageContainer.hpp"
#include "ImageContainer.hpp"
#include "RegExp.hpp"
//#include "HTTPHeader.hpp"
//#include "NaughtyFilter.hpp"
#include "StoryBoard.hpp"
#include <string>
#include <deque>

// DECLARATIONS

std::deque<String> *ipToHostname(const char *ip);

void getClientFromIP(const char *ip, std::string &clienthost);

class FOptionContainer {

public:
    int reporting_level;
    int category_threshold;
    bool infection_bypass_errors_only;
    bool disable_content_scan;
    bool disable_content_scan_error;
    bool content_scan_exceptions;
    bool automitm;
    int weighted_phrase_mode;
    unsigned int banned_phrase_list;
    int group_mode;
    int embedded_url_weight;
    int naughtyness_limit;
    int searchterm_limit;
    off_t max_upload_size;
    int filtergroup;
    bool non_standard_delimiter;

    //SSL certificate checking
    bool ssl_check_cert;

    //SSL Man in the middle
    bool ssl_mitm;
    bool only_mitm_ssl_grey;
    bool mitm_check_cert;

#ifdef ENABLE_EMAIL
    // Email notification patch by J. Gauthier
    bool notifyav;
    bool notifycontent;
    bool use_smtp;
    int violations;
    int current_violations;
    int threshold;
    long threshold_stamp;
    bool byuser;
#endif

    bool reverse_lookups;
    bool force_quick_search;
    int bypass_mode;
    int infection_bypass_mode;

    std::string name;
    std::string magic;
    std::string imagic;
    std::string cookie_magic;

#ifdef ENABLE_EMAIL
    // Email notification patch by J. Gauthier
    std::string mailfrom;
    std::string avadmin;
    std::string contentadmin;
    std::string avsubject;
    std::string contentsubject;
    std::string violationbody;
#endif
#ifdef PRT_DNSAUTH
    unsigned int auth_exception_site_list;
    unsigned int auth_exception_url_list;
#endif
    bool use_only_local_allow_lists;

    bool url_redirect_regexp_flag;
    bool allow_empty_host_certs;


    // regex search & replace lists
    std::deque <RegExp> content_regexp_list_comp;
    std::deque <String> content_regexp_list_rep;
    RegExp isiphost;

    // access denied address & domain - if they override the defaults
    std::string access_denied_address;
    std::string sslaccess_denied_address;
    String access_denied_domain;
    String sslaccess_denied_domain;
    bool ssl_denied_rewrite;
    // search term blocking
    unsigned int searchterm_list;
    bool searchterm_flag;

    FOptionContainer()
            :  searchterm_flag(false), banned_page(NULL), neterr_page(NULL), ssl_mitm(false),
              only_mitm_ssl_grey(false), ssl_check_cert(false), mitm_check_cert(true),
              banned_phrase_flag(false),
              content_regexp_flag(false),
              ssl_denied_rewrite(false) {};

    ~FOptionContainer();
    bool read(const char *filename);
    bool readConfFile(const char *filename);
    void reset();
    void resetJustListData();

    bool isOurWebserver(String url);
#ifdef PRT_DNSAUTH
    bool inAuthExceptionSiteList(String url, bool doblanket = false, bool ip = false, bool ssl = false);
    bool inAuthExceptionURLList(String url, bool doblanket = false, bool ip = false, bool ssl = false);
#endif



    StoryBoard StoryB;

    // get HTML template for this group
    HTMLTemplate *getHTMLTemplate(bool upfail);
    std::deque<std::string> text_mime;

    private:
    // HTML template - if it overrides the default
    HTMLTemplate *banned_page;
    HTMLTemplate *neterr_page;

    ListMeta LMeta;

    bool banned_phrase_flag;
    bool content_regexp_flag;
#ifdef PRT_DNSAUTH
    bool auth_exception_site_flag;
    bool auth_exception_url_flag;
#endif
    bool banned_search_flag;

    std::deque<int> banned_phrase_list_index;

    std::deque<std::string> conffile;

    bool precompileregexps();
    bool readFile(const char *filename, unsigned int *whichlist, bool sortsw, bool cache, const char *listname);
    bool compileRegExMatchFile(unsigned int list, std::deque<RegExp> &list_comp,
        std::deque<String> &list_source, std::deque<unsigned int> &list_ref);
    bool readRegExReplacementFile(const char *filename, const char *listname, unsigned int &listid,
        std::deque<String> &list_rep, std::deque<RegExp> &list_comp);

    int findoptionI(const char *option);
    std::string findoptionS(const char *option);
    std::deque<String> findoptionM(const char *option);
    bool realitycheck(int l, int minl, int maxl, const char *emessage);
    int inRegExpURLList(String &url, std::deque<RegExp> &list_comp, std::deque<unsigned int> &list_ref, unsigned int list, String &lastcategory);

    char *inURLList(String &url, unsigned int list, bool doblanket , bool ip , bool ssl , String &lastcategory);
    char *inSiteList(String &url, unsigned int list, bool doblanket , bool ip , bool ssl , String &lastcategory);

    char *testBlanketBlock(unsigned int list, bool ip, bool ssl, String &lastcategory);
};

#define __HPP_FOPTIONCONTAINER
#endif
