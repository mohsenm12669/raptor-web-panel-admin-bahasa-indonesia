#include <iostream>
#include <cstring>
#include <vector>
#include "../utils.cpp"

using namespace std;

// use this line to compile
// g++ -I. -fPIC -shared -g -o microsoft.com.so microsoft.com.cpp
// regex
//http.*\.microsoft\.com.*(\.cab|\.exe|\.iso|\.zip|\.psf|\.msu|\.msi|\.msp|\.dsft)

bool in_array(const string &needle, const vector< string > &haystack) {
    int max = haystack.size();

    if (max == 0) return false;
        
    for (int iii = 0; iii < max; iii++) {
           if (regex_match(haystack[iii], needle) != "") {
               return true;
        }
    }
    return false;
}

string get_filename(string url) {
        vector<string> resultado;
        if (url.find("?") != string::npos){
            stringexplode(url, "?", &resultado);
            url = resultado.at(resultado.size()-2);
        }
    
        if (regex_match("[0-9]{10}$", url) != "") {
            
            stringexplode(url, "/", &resultado);
            return resultado.at(resultado.size()-3); 
        } else {
            stringexplode(url, "/", &resultado);
            return resultado.at(resultado.size()-1);
        }            
}

extern "C" resposta getmatch(const string url) {
    resposta r;
    
    vector<string> black_list; 
    black_list.push_back ("WuSetupHandler.cab");
    black_list.push_back ("muv3muredir.cab");
    black_list.push_back ("muredir.cab");
    black_list.push_back ("wuredir.cab");
    black_list.push_back ("muauth.cab");
    black_list.push_back ("musetup.cab");
    black_list.push_back ("wsus3setup.cab");
    black_list.push_back ("authrootstl.cab");
    black_list.push_back ("authrootseq.txt");
    black_list.push_back ("default.aspx");
    black_list.push_back ("wuident.cab");
    black_list.push_back ("\\.asmx");
    black_list.push_back ("stats\\.");
    black_list.push_back ("\\.asp");
        black_list.push_back ("cdm.cab");

    if (   (url.find(".microsoft.com") != string::npos) and
        ( (url.find(".cab") != string::npos) or (url.find(".exe") != string::npos) or (url.find(".iso") != string::npos) or
                     (url.find(".zip") != string::npos) or (url.find(".psf") != string::npos) or (url.find(".msi") != string::npos) or
                     (url.find(".msp") != string::npos) or (url.find(".msu") != string::npos) or (url.find(".dsft") != string::npos) ) and
            (in_array(url, black_list) == false)
     {

        r.file = get_filename(url);
        if (!r.file.empty()) {
            r.match = true;
            r.domain = "microsoftupdate";
        } else {
            r.match = false;
        }
    } else {
        r.match = false;
    }
    return r;
}