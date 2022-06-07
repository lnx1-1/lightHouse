#include <iostream>
#include <string>

#include <regex>

using namespace std;

string changeSpeedValue(string html, int newVal);

int main(int argc, char const *argv[]) {

    int replacementValue = 100;

    string indexHTML = R"(<input type="text" id="message" name="Message" value="">
        <div class="slidecontainer">
          <div style = "text-align: center;">Speed:</div>
          <input type="range" min="100" max="1000" name="morseSpeed" value="500" class="slider" id="myRange">
        </div>)";

    cout << changeSpeedValue(indexHTML, 100) << endl;
    cout << changeSpeedValue(indexHTML, 500) << endl;
    cout << changeSpeedValue(indexHTML, 1000) << endl;

    return 0;
}

string changeSpeedValue(string html, int newVal) {
    regex reg("(name=\"morseSpeed\" value=)\"\\d*(\".*?)");
    string replacementString = "$1\"" + to_string(newVal) + "$2";
    html = regex_replace(html, reg, replacementString);

    string replacementString2 = "$1 " + to_string(newVal) + "$2";
    regex reg2("(<div style = \"text-align: center;\">Speed:).*?(</div>).*?");
    html = regex_replace(html, reg2, replacementString2);
    return html;
}
