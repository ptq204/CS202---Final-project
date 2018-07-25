#include"stdafx.h"
#include "function.h"
vector<Shape*> Shape::sampleObjects;
vector<float> splitRGB(string s) {
	vector<float> v;
	stringstream ss(s);
	string tmp;
	getline(ss, tmp, '(');
	getline(ss, tmp, ',');
	v.push_back(stof(tmp));
	getline(ss, tmp, ',');
	v.push_back(stof(tmp));
	getline(ss, tmp, ',');
	v.push_back(stof(tmp));
	return v;
}
void splitTransform(Graphics& graphics, string s) {
	string input = s;
	stringstream removeSpace(input); s = "";
	while (removeSpace >> input) {
		if (!s.empty()) s += " ";
		s += input;
	}
	stringstream ss(s);
	string token;
	while (getline(ss, token, '(')) {
		stringstream removeSpace(token);
		removeSpace >> token;
		string tmp = token.substr(0, 9);
		if (tmp == "translate") {
			getline(ss, token, ',');
			float dx = stof(token);
			getline(ss, token, ')');
			float dy = stof(token);
			cout << "trans:" << dx << " " << dy << endl;
			graphics.TranslateTransform(dx, dy);
		}
		else {//rotate
			tmp = tmp.substr(0, 6);
			if (tmp == "rotate") {
				getline(ss, token, ')');
				cout << "rotate:" << token << endl;
				graphics.RotateTransform(stof(token));
			}
			else {//scale
				getline(ss, token, ')');
				stringstream ss2(token);
				getline(ss2, token, ',');
				float x = stof(token);
				ss2 >> token;
				float y = stof(token);
				cout << "scale:" << x << " " << y << endl;
				graphics.ScaleTransform(x, y);
			}
		}
	}
}
void splitPath(string s) {
	stringstream ss1(s);
	string token;
	vector<PointF> v;
	bool isFirst = true, isLast = false;
	PointF first, last;
	while (ss1 >> token) {
		if (token[0] == 'M') {
			token = token.substr(1);
			stringstream ss2(token);
			getline(ss2, token, ',');
			float x = stof(token);
			ss2 >> token;
			float y = stof(token);
			if (isFirst) {
				first.X = x;
				first.Y = y;
				isFirst = false;
			}
			last.X = x;
			last.Y = y;
		}
		else if (token[0] == 'C') {
			cout << "C:" << endl;
			token = token.substr(1);
			vector<PointF> Bezier;
			Bezier.push_back(first);
			for (int i = 0; i < 3; ++i) {
				stringstream ss2(token);
				getline(ss2, token, ',');
				float x = stof(token);
				ss2 >> token;
				float y = stof(token);
				PointF tmp(x, y);
				Bezier.push_back(tmp);
				if (i != 2) ss1 >> token;
				else {//i==2
					if (isLast == false) {
						isLast = true;
						last = tmp;
					}
				}
			}
		}
	}
}
vector<PointF> splitPoint(string s) {
	stringstream ss1(s);
	string token;
	vector<PointF> v;
	PointF p;
	while (ss1 >> token) {
		stringstream ss2(token);
		getline(ss2, token, ',');
		p.X = stof(token);
		ss2 >> token;
		p.Y = stof(token);
		cout << "points:" << p.X << " " << p.Y << endl;
		v.push_back(p);
	}
	return v;
}
vector<float> Attribute::getArrValue()
{
	return arr;
}
float Attribute::getValue() {
	return value;
}
vector<PointF> Attribute::getArrPoint() {
	return Points;
}
string Attribute::getStringValue() {
	return transformList;
}
void stroke_color::setValue(string s) {
	cout << "STRING S:" << s << endl;
	if (s == "none") {
		arr[0] = arr[1] = arr[2] = 256;
		return;
	}
	vector<float> v = splitRGB(s);
	for (int i = 0; i < v.size(); i++)
		arr[i] = v[i];
}
void fill_color::setValue(string s)
{
	cout << "STRING S:" << s << endl;
	if (s == "none") {
		arr[0] = arr[1] = arr[2] = 256;
		return;
	}
	vector<float> v = splitRGB(s);
	for (int i = 0; i < v.size(); i++) {
		arr[i] = v[i];
		AllocConsole(); freopen("CONOUT$", "w", stdout);
		cout << "arr:" << arr[i] << endl;
	}
}
void transform::setValue(string s)
{
	transformList += (" " + s);
	cout << "transform list:" << transformList << endl;
}
void Shape::addSample(Shape* pFig) {
	if (pFig == NULL) return;
	int pos = sampleObjects.size();
	while (--pos >= 0) {
		Shape* Obj = sampleObjects[pos];
		if (pFig->getName() == Obj->getName()) break;
	}
	if (pos < 0) {
		sampleObjects.push_back(pFig);
	}
}
Shape::Shape()
{
	Attribute* a;
	a = new x1;//0
	attributes.push_back(a);
	a = new y_1;//1
	attributes.push_back(a);
	a = new x2;//2
	attributes.push_back(a);
	a = new y2;//3
	attributes.push_back(a);
	a = new x;//4
	attributes.push_back(a);
	a = new y;//5
	attributes.push_back(a);
	a = new cx;//6
	attributes.push_back(a);
	a = new cy;//7
	attributes.push_back(a);
	a = new rx;//8
	attributes.push_back(a);
	a = new ry;//9
	attributes.push_back(a);
	a = new r;//10
	attributes.push_back(a);
	a = new width;//11
	attributes.push_back(a);
	a = new height;//12
	attributes.push_back(a);
	a = new stroke_width;//13
	attributes.push_back(a);
	a = new stroke_opacity;//14
	attributes.push_back(a);
	a = new fill_opacity;//15
	attributes.push_back(a);
	a = new stroke_color;//16
	attributes.push_back(a);
	a = new fill_color;//17
	attributes.push_back(a);
	a = new transform;//18
	attributes.push_back(a);
	a = new d;//19
	attributes.push_back(a);
	a = new points;
	attributes.push_back(a);
	a = new TextLine;
	attributes.push_back(a);
	a = new FontSize;
	attributes.push_back(a);
}
void Shape::setAttribute(string attr_name, string attr_value)
{
	int size = this->attributes.size();
	for (int i = 0; i < size; i++) {
		if (attr_name == this->attributes[i]->getName()) {
			this->attributes[i]->setValue(attr_value);
			return;
		}
	}
}
Shape* Shape::createObject(string clsName) {
	int pos = sampleObjects.size();
	while (--pos >= 0) {
		Shape* Obj = sampleObjects[pos];
		if (Obj == NULL) continue;
		if (clsName == Obj->getName()) break;
	}
	if (pos >= 0) {
		return sampleObjects[pos]->Clone();
	}
	return NULL;
}
Pen* Shape::getPen() {
	vector<float> v = attributes[STROKE_COLOR]->getArrValue();
	if (v[0] == 256) attributes[STROKE_OPACITY]->setValue("0");
	if (attributes[STROKE_OPACITY]->getValue() == 0 && attributes[STROKE_WIDTH]->getValue() != 0)
		attributes[STROKE_OPACITY]->setValue("1");
	Pen pen(Color(attributes[STROKE_OPACITY]->getValue() * 255, v[0], v[1], v[2]), attributes[STROKE_WIDTH]->getValue());
	cout << attributes[STROKE_OPACITY]->getValue() << " " << v[0] << " " << v[1] << " " << v[2] << endl;
	return pen.Clone();
}
Brush* Shape::getBrush() {
	vector<float> v = attributes[FILL_COLOR]->getArrValue();
	if (v[0] == 256) attributes[FILL_OPACITY]->setValue("0");
	SolidBrush Brush(Color(attributes[FILL_OPACITY]->getValue() * 255, v[0], v[1], v[2]));
	cout << attributes[FILL_OPACITY]->getValue() << " " << v[0] << " " << v[1] << " " << v[2] << endl;
	return Brush.Clone();
}
void Shape::Input(vector<Attribute*> old, xml_node<> *&proot) {
	if (old.size()) copyVector(old, attributes);
	AllocConsole(); freopen("CONOUT$", "w", stdout); cout << "textline:" << proot->value() << endl;
	attributes[TEXTLINE]->setValue(proot->value());
	xml_attribute<>* attr = proot->first_attribute();
	for (attr; attr; attr = attr->next_attribute())//input of each
	{
		AllocConsole(); freopen("CONOUT$", "w", stdout);
		cout << "attri:" << attr->name() << " " << attr->value() << endl;
		this->setAttribute(attr->name(), attr->value());
	}
}
//GROUP
void Group::Input(vector<Attribute*> old, xml_node<> *&proot) {//first all
	Shape* s;
	xml_node<>* pnode = proot->first_node();//chua input lay chinh g
	Shape::Input(old, proot);
	for (pnode; pnode; pnode = pnode->next_sibling())
	{
		string tmp = pnode->name();
		s = Shape::createObject(tmp);
		s->Input(attributes, pnode);
		arr.push_back(s);
	}
}
//GROUP
//LINE
VOID Line::Draw(HDC hdc) {
	Graphics graphics(hdc);
	float x1 = attributes[X1]->getValue();
	float y1 = attributes[Y1]->getValue();
	float x2 = attributes[X2]->getValue();
	float y2 = attributes[Y2]->getValue();
	splitTransform(graphics, attributes[TRANSFORM]->getStringValue());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.DrawLine(getPen(), x1, y1, x2, y2);
}
//LINE
//RECT
VOID HCN::Draw(HDC hdc) {
	Graphics graphics(hdc);
	float x = attributes[X]->getValue();
	float y = attributes[Y]->getValue();
	float width = attributes[WIDTH]->getValue();
	float height = attributes[HEIGHT]->getValue();
	splitTransform(graphics, attributes[TRANSFORM]->getStringValue());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.FillRectangle(getBrush(), x, y, width, height);
	graphics.DrawRectangle(getPen(), x, y, width, height);
}
//RECT
//ELIP
VOID Elip::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	float rx = attributes[RX]->getValue();
	float ry = attributes[RY]->getValue();
	float cx = attributes[CX]->getValue();
	float cy = attributes[cY]->getValue();
	splitTransform(graphics, attributes[TRANSFORM]->getStringValue());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.FillEllipse(getBrush(), cx - rx, cy - ry, rx * 2, ry * 2);
	graphics.DrawEllipse(getPen(), cx - rx, cy - ry, rx * 2, ry * 2);
}
//ELIP
//CIRCLE
VOID Circle::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	float r = attributes[R]->getValue();
	float cx = attributes[CX]->getValue();
	float cy = attributes[cY]->getValue();
	splitTransform(graphics, attributes[TRANSFORM]->getStringValue());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.FillEllipse(getBrush(), cx - r, cy - r, r * 2, r * 2);
	graphics.DrawEllipse(getPen(), cx - r, cy - r, r * 2, r * 2);
}
//CIRCLE
//POLYLINE
VOID PolyLine::Draw(HDC hdc) {
	Graphics graphics(hdc);
	GraphicsPath polypath;
	vector<PointF> v = attributes[PS]->getArrPoint();
	polypath.AddLines(&v[0], v.size());
	splitTransform(graphics, attributes[TRANSFORM]->getStringValue());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.FillPath(getBrush(), &polypath);
	graphics.DrawLines(getPen(), &v[0], v.size());
}
//POLYLINE
//POLYGON
VOID DaGiac::Draw(HDC hdc) {
	Graphics graphics(hdc);
	vector<PointF> v = attributes[PS]->getArrPoint();
	splitTransform(graphics, attributes[TRANSFORM]->getStringValue());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.FillPolygon(getBrush(), &v[0], v.size());
	graphics.DrawPolygon(getPen(), &v[0], v.size());
}
//POLYGON
//PATH
VOID Path::Draw(HDC hdc) {
	Graphics graphics(hdc);
	GraphicsPath path;
	string s = attributes[D]->getStringValue();
	stringstream ss1(s);
	string token;
	PointF first, last;
	while (ss1 >> token) {
		if (token[0] == 'M') {
			token = token.substr(1);
			stringstream ss2(token);
			getline(ss2, token, ',');
			float x = stof(token);
			ss2 >> token;
			float y = stof(token);
			first.X = last.X = x;
			first.Y = last.Y = y;
			cout << "M:" << x << " " << y << endl;
			path.StartFigure();
		}
		else if (token[0] == 'H' || token[0] == 'h') {
			token = token.substr(1);
			stringstream ss2(token);
			while (getline(ss2, token, ',')) {
				float x = stof(token);
				//graphics.DrawLine(getPen(), last.X, last.Y, x, last.Y);
				path.AddLine(PointF(last.X, last.Y), PointF(x, last.Y));
				cout << "H:" << last.X << " " << last.Y << " " << x << " " << last.Y << endl;
				last.X = x;
			}
		}
		else if (token[0] == 'V' || token[0] == 'v') {
			token = token.substr(1);
			stringstream ss2(token);
			while (getline(ss2, token, ',')) {
				float y = stof(token);
				//graphics.DrawLine(getPen(), last.X, last.Y, last.X, y);
				path.AddLine(PointF(last.X, last.Y), PointF(last.X, y));
				cout << "V:" << last.X << " " << last.Y << " " << last.X << " " << y << endl;
				last.Y = y;
			}
		}
		else if (token[0] == 'L') {
			token = token.substr(1);
			stringstream ss2(token);
			getline(ss2, token, ',');
			float x = stof(token);
			ss2 >> token;
			float y = stof(token);
			//graphics.DrawLine(getPen(), last.X, last.Y, x, y);
			path.AddLine(PointF(last.X, last.Y), PointF(x, y));
			cout << "DRAW LINE L:" << last.X << " " << last.Y << " " << x << " " << y << endl;
			last.X = x;
			last.Y = y;
		}
		else if (token[0] == 'C') {
			token = token.substr(1);
			vector<PointF> Bezier;
			Bezier.push_back(last);
			for (int i = 0; i < 3; ++i) {
				stringstream ss2(token);
				getline(ss2, token, ',');
				float x = stof(token);
				ss2 >> token;
				float y = stof(token);
				PointF tmp(x, y);
				Bezier.push_back(tmp);
				if (i != 2) ss1 >> token;
			}
			last.X = Bezier[3].X;
			last.Y = Bezier[3].Y;
			//graphics.DrawBeziers(getPen(), &Bezier[0], 4);
			path.AddBezier(Bezier[0].X, Bezier[0].Y, Bezier[1].X, Bezier[1].Y,
				Bezier[2].X, Bezier[2].Y, Bezier[3].X, Bezier[3].Y);
		}
		else if (token == "Z") {
			//graphics.DrawLine(getPen(), first.X, first.Y, last.X, last.Y);
			path.AddLine(PointF(first.X, first.Y), PointF(last.X, last.Y));
			cout << "Z:" << first.X << " " << first.Y << " " << last.X << " " << last.Y << endl;
			last.X = first.X;
			last.Y = first.Y;
		}
	}
	splitTransform(graphics, attributes[TRANSFORM]->getStringValue());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.FillPath(getBrush(), &path);
	graphics.DrawPath(getPen(), &path);
}
//POLYGON
//TEXT
VOID Text::Draw(HDC hdc) {
	Graphics graphics(hdc);
	string thin = attributes[TEXTLINE]->getStringValue();
	wstring wide(thin.begin(), thin.end());
	float fontsize = attributes[FONTSIZE]->getValue();
	Font font(L"Times New Roman", fontsize, 0, UnitPixel);
	FontFamily familyFont(L"Times New Roman");
	PointF point(attributes[X]->getValue() - 25, attributes[Y]->getValue() - fontsize + 5);
	splitTransform(graphics, attributes[TRANSFORM]->getStringValue());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.DrawString(wide.c_str(), -1, &font, point, getBrush());
	vector<float> v = attributes[STROKE_COLOR]->getArrValue();
	if (v[0] == 255 && v[1] == 255 && v[2] == 255) return;
	GraphicsPath path;
	path.AddString(wide.c_str(), -1, &familyFont, 0, fontsize, point, NULL);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.DrawPath(getPen(), &path);
}
//TEXT
void copyVector(vector<Attribute*>&old, vector<Attribute*>& attributes) {
	if (old.size() == 0) return;
	attributes[FILL_OPACITY]->setValue(to_string(old[FILL_OPACITY]->getValue()));
	attributes[STROKE_OPACITY]->setValue(to_string(old[STROKE_OPACITY]->getValue()));
	attributes[STROKE_WIDTH]->setValue(to_string(old[STROKE_WIDTH]->getValue()));
	vector<float> v = old[FILL_COLOR]->getArrValue();
	string rgb = "rgb(" + to_string(v[0]) + ", " + to_string(v[1]) + ", " + to_string(v[2]) + ")";
	attributes[FILL_COLOR]->setValue(rgb);
	v = old[STROKE_COLOR]->getArrValue();
	rgb = "rgb(" + to_string(v[0]) + ", " + to_string(v[1]) + ", " + to_string(v[2]) + ")";
	attributes[STROKE_COLOR]->setValue(rgb);
	attributes[TRANSFORM]->setValue(old[TRANSFORM]->getStringValue());
}

void inputList(Group &g)
{
	Line a1; HCN a2; Circle a3; Elip a4; PolyLine a5; DaGiac a6; Path a7; Group a8;
	Text a9;
	xml_document<> doc;
	ifstream file("lion.svg");
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	string content(buffer.str());
	doc.parse<0>(&content[0]);
	xml_node<> *proot = doc.first_node();
	vector<Attribute*> dummy;
	g.Input(dummy, proot);
}