#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include <regex>
#include <utility>
#include <windows.h>
#include <curl/curl.h>
#include <time.h>

#pragma execution_character_set( "utf-8" )




namespace MyParser {


	enum View {
		body, div, a, h, p, img, video, ul, span, li, h1, h2, h3, tab, nav, input, button, form, section, label, article, style, b, code, iframe, script, option, path, svg, ol, em, unknown
	};

	class Element {

	private:
		std::string seperator_src = "src=\"";
		std::string seperator_id = "id=\"";
		std::string seperator_class = "class=\"";
		std::string seperator_href = "href=\"";
		bool src_split = false;
		bool id_split = false;
		bool class_split = false;
		bool href_split = false;
		std::string src = "";
		std::string id = "";
		std::string classname = "";
		std::string href = "";
		std::string title = "";
		int srcpos = 0;
		int idpos = 0;
		int classpos = 0;
		int hrefpos = 0;
		bool typedone = false;

	public:
		Element(std::string _header, int _dispo, int _parent, int _pos) {
			header = _header;
			dispostufe = _dispo;
			parent = _parent;
			pos = _pos;





			for (int i = 0; i < header.size(); i++) {
				if (!typedone && (header[i] == ' ' || header[i] == '>')) {
					auto typestr = header.substr(1, i - 1);
					type = hashit(typestr);

					typedone = true;
				}
				//src
				if (header[i] == seperator_src[0]) {
					if (header.substr(i, seperator_src.size()) == seperator_src) {
						srcpos = i + seperator_src.size();
						i = srcpos;
						src_split = true;
					}
				}

				if (src_split && header[i] == '\"') {

					src = header.substr(srcpos, i - srcpos);
					src_split = false;
				}
				//id

				if (header[i] == seperator_id[0]) {
					if (header.substr(i, seperator_id.size()) == seperator_id) {
						idpos = i + seperator_id.size();
						i = idpos;
						id_split = true;
					}
				}

				if (id_split && header[i] == '\"') {

					id = header.substr(srcpos, i - srcpos);
					id_split = false;
				}


				//class
				if (header[i] == seperator_class[0]) {
					if (header.substr(i, seperator_class.size()) == seperator_class) {
						classpos = i + seperator_class.size();
						i = classpos;
						class_split = true;
					}
				}

				if (class_split && header[i] == '\"') {

					classname = header.substr(classpos, i - classpos);
					class_split = false;
				}
				//href
			/*	if(header[i] == seperator_href[0]){
					if(header.substr(i,seperator_href.size()) == seperator_href){
						classpos = i + seperator_href.size();
						i = hrefpos ;
						href_split = true ;
					}
				}

				if(href_split && header[i] == '\"'){

						href = header.substr(hrefpos, i - hrefpos);
						href_split = false ;
				}*/
			}
		}
		Element(
		) {
		}

		std::vector<Element*> getChildren() {

			return  children;


		}

		Element* getChild() {

			return children.empty() ? NULL : children[0];
		}

		std::string getSrc() {
			return src;
		}

		std::string getClass() {
			return classname;
		}

		std::string getId() {
			return id;
		}


		std::string getHref() {
			return href;
		}

		View hashit(std::string const& inString) {
			if (inString == "div") return div;
			if (inString == "body") return body;
			if (inString == "a") return a;
			if (inString == "h") return h;
			if (inString == "p") return p;
			if (inString == "img") return img;
			if (inString == "video") return video;
			if (inString == "li") return li;
			if (inString == "ul") return ul;
			if (inString == "span") return span;
			if (inString == "h2") return h2;
			if (inString == "h3") return h3;
			if (inString == "tab") return tab;
			if (inString == "nav") return nav;
			if (inString == "input") return input;
			if (inString == "button") return button;
			if (inString == "form") return form;
			if (inString == "section") return section;
			if (inString == "label") return label;
			if (inString == "article") return article;
			if (inString == "style") return style;
			if (inString == "b") return b;
			if (inString == "code") return code;
			if (inString == "iframe") return iframe;
			if (inString == "script") return script;
			if (inString == "option") return option;
			if (inString == "path") return path;
			if (inString == "svg") return svg;
			if (inString == "ol") return ol;
			if (inString == "em") return em;
			if (inString == "h1") return h1;



			return  unknown;
		}
		int pos = -1;
		std::string content = "";
		std::string header;
		int dispostufe;
		int parent;
		View type;
		std::vector<MyParser::Element* > children;

	};







	/*
**Does get alll times but doesnt resolve dependencies
*/

	std::vector<Element* > split(std::string data, std::string begin, std::string seperator1, std::string seperator2, std::string seperator3) {


		std::string whitelist[] = { "!", "img","input","meta","link" , "br" , "area" , "base" ,"col" , "command" , "embed","hr" , "keygen" , "param", "source" , "track" , "wbr" , "!Doctype" };
		 
		int len1 = seperator1.length();
		int len2 = seperator2.length();
		int len3 = seperator3.length();
		int beginlen = begin.length();
		std::vector<Element* > output;
		bool sep = false;
		int counter = -1;
		int lastpos = 0;
		bool ignore = false;
		std::vector<int>  lastpositions;
		std::vector<int>  itempos;

		int seppos = -1;


		//	std::vector<int>  hendpositions;

		for (int i = 0; i < data.length(); i++) {

			if (!ignore && data[i] == begin[0])
				if (data.substr(i, beginlen) == begin && data.substr(i, len1) != seperator1) {
					lastpositions.push_back(i);
					counter++;

				}

			if (sep == false && lastpositions.size() > 0 && !ignore &&  data[i] == seperator2[0])
				if (data.substr(i, len2) == seperator2)
				{
					int pos = (i + len2);
					//		hendpositions.push_back(i);
					lastpos = lastpositions.back();

					int parentid = -1;
					int position = output.size();
					if (position > 0) {
						auto lastitem = output.back();
						parentid = (lastitem->dispostufe == counter) ? lastitem->parent : parentid;
						parentid = (lastitem->dispostufe < counter) ? output.size() - 1 : parentid;
						while (lastitem->dispostufe > counter) {
							lastitem = output[lastitem->parent];
							parentid = lastitem->parent;

						}


					}
					 

					output.push_back(new Element(data.substr(lastpos, pos - lastpos), counter, parentid, position));

					if (parentid != -1) {
						output[parentid]->children.push_back(output[position]);
					}

					lastpositions.back() = i + 1;
					itempos.push_back(output.size() - 1);
					for (std::string item : whitelist) {
						if (data.substr(lastpos + 1, item.length()) == item) {
							/*	lastpos = lastpositions.back() ;
								lastpositions.pop_back() ;
									counter--;*/
							sep = true;
						}

					}
					ignore = false;
					std::string igno = "script";
					if (data.substr(lastpos + 1, igno.length()) == igno) {
						ignore = true;
					}
					 

				}

			if (sep && data[i] == seperator2[0])
			{
				if (data.substr(i, len2) == seperator2)
				{

					/*	if(counter > 1){
							counter--;
							sep = false ;
							 continue;
						}*/

					int pos = i; //(i + len2);
					lastpos = lastpositions.back();
					lastpositions.pop_back();
					auto stri = data.substr(lastpos, seppos - lastpos);

					//		std::cout << lastpos << "::" << pos << " Output: " << stri << std::endl;

					auto itempos1 = itempos.back();
					itempos.pop_back();

					output[itempos1]->content = (stri);

					ignore = false;

					lastpos = pos + 1;
					counter--;
					sep = false;
				}
			}


			if (data[i] == seperator3[0])
				if (data.substr(i, len3) == seperator3)
				{
					/*		if(counter > 1){
								counter--;
								sep = false ;
								 continue;
							}*/

					lastpos = lastpositions.back();
					lastpositions.pop_back();
					//	int pos = (i + len3);
					int pos = i;
					auto stri = data.substr(lastpos, pos - lastpos);

					//std::cout << lastpos << "::" << pos << " Output: " << stri << std::endl;


					auto itempos1 = itempos.back();
					itempos.pop_back();

					//		output[itempos1].content = (stri);	

					ignore = false;

					lastpos = pos + 1;
					counter--;
					sep = false;
				}

			if (counter >= 0 && data[i] == seperator1[0])
				if (data.substr(i, len1) == seperator1) {//first char equals 
					sep = true;
					seppos = i;
				}
		}
		std::cout << "Counter: " << counter << std::endl;
		std::cout << "itempos: " << itempos.size() << std::endl;
		return output;
	}



	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}

	std::string get_response(std::string url)
	{
		CURL* curl;
		CURLcode res;
		std::string readBuffer;

		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
		}


		return readBuffer;
	}
	 

	class HtmlParser {

	public:

		HtmlParser() {

		}

		std::vector<Element* > Elements;
		void setup(std::string url) {
			std::string html = get_response(url);
			Elements = MyParser::split(html, "<", "</", ">", "/>");
			
		}
		void setup_string(std::string html) {
			
			Elements = MyParser::split(html, "<", "</", ">", "/>");

		}
	};


 


}




void printItem(MyParser::Element item) {
	std::cout << "Type: " << item.type << " Header : " << item.header << " Count: " << item.dispostufe << " Content : " << (item.content == "") << " Parent Position : " << item.parent << std::endl;
}



int main() {
	
	
	SetConsoleOutputCP(65001);
	clock_t tStart = clock();
	


	auto parser = MyParser::HtmlParser();

	parser.setup("https://serien.me/serien");

	auto list = parser.Elements;
	
 	std::cout << "List items: " << list.size() << std::endl;

	  for (int i = 0; i < list.size(); i++) {
		//	std::cout << "Type: " << item.type << std::endl ;
		auto item = list[i];
		if (item->type == MyParser::View::div)
			if (item->getClass() == "genre")
			{
				auto childs = item->children[1]->children;
				if (childs.size()>0 )
					std::cout << "Item : " << item->children[0]->children[0]->content << " Childrens : " << childs[0]->children[0]->content << std::endl;
			}
	} 
	printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	return 0;
}


