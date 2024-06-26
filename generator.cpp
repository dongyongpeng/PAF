#include "generator.h"

using namespace std;

void generator::GenSubList()       
{
    for (int i = 0; i < subs; i++)
    {
        Sub sub = GenOneSub(i);                   //Sub GenOneSub(int id)

        subList.push_back(sub);
    }

    ofstream fileStream;
    fileStream.open("./Detail/subList.txt");
    for (int i=0; i<subs; ++i)
    {
        Sub &sub = subList[i];
        fileStream << "sub" << sub.id << "\t" << sub.size << "\t";
        for (int j=0; j<sub.size; ++j)
            fileStream << sub.constraints[j].att << OPR[sub.constraints[j].op] << sub.constraints[j].value << "\t";
        fileStream << "\n";
    }
    fileStream.close();
}


Sub generator::GenOneSub(int id)  //What is the basis for choosing this distribution?
{
    Sub sub;
    sub.id = id;
    sub.size = random(k) + 1;   //from 1 
    if (subAttDis == 0)   //what is the subAttDis?
        GenUniformAtts(sub);
    else if (subAttDis==1)
        GenZipfAtts(sub);
    if (subValDis == 0)
        GenUniformValues(sub);
    else if (subValDis == 1)
        GenZiefValues(sub);
    return sub;
}

void generator::GenUniformAtts(Sub &sub)
{
    vector<int> a;                  //what do a and x represents for?
    for (int i=0; i<sub.size; ++i)
    {
        int x = random(attDom);     //attDom is attribute domain?
        while (CheckExist(a,x))
            x = random(attDom);
        Cnt tmp;
        tmp.att = x;
        sub.constraints.push_back(tmp);
    }
}

void generator::GenZipfAtts(Sub &sub)
{
    vector<int> a;
    first = true;
    for (int i = 0; i < sub.size; i++)
    {
        int x = zipfDistribution(attDom, subAttalpha);
        while (CheckExist(a,x))
            x = zipfDistribution(attDom, subAttalpha);
        a.push_back(x);
        Cnt tmp;
        tmp.att = x;
        sub.constraints.push_back(tmp);
    }
}

void generator::GenUniformValues(Sub &sub)
{
    for (int i = 0; i < sub.size; i++)
    {
        int x = random(valDom);
        sub.constraints[i].value = x;
        double y = rand()/(RAND_MAX + 1.0);
        if (y <equalRatio)
            sub.constraints[i].op = 0;
        else if (y>(1+equalRatio)/2)
            sub.constraints[i].op = 2;
        else
            sub.constraints[i].op = 1;
    }
}

void generator::GenZiefValues(Sub &sub)
{
    first = true;
    for (int i = 0; i < sub.size; i++)
    {
        int x = zipfDistribution(valDom, subValalpha);
        sub.constraints[i].value = x;
        double y = rand()/(RAND_MAX + 1.0);
        if (y <equalRatio)
            sub.constraints[i].op = 0;
        else if (y>(1+equalRatio)/2)
            sub.constraints[i].op = 2;
        else
            sub.constraints[i].op = 1;
    }
}


void generator::GenPubList()
{
    for (int i = 0; i < pubs; i++)
    {
        Pub pub = GenOnePub();
        pubList.push_back(pub);
    }
    ofstream fileStream;
    fileStream.open("./Detail/pubList.txt");
    for (int i = 0; i < pubs; i++)
    {
        Pub &pub = pubList[i];
        fileStream << "pub" << i << "\t";
        for (int j=0; j<pub.size; ++j)
            fileStream << pub.pairs[j].att << "=" << pub.pairs[j].value << "\t";
        fileStream << "\n";
    }
    fileStream.close();
}


Pub generator::GenOnePub()
{
    Pub pub;
    pub.size = m;
    if (pubAttDis == 0)
        GenUniformAtts(pub);
    else if (pubAttDis == 1)
        GenZipfAtts(pub);
    if (pubValDis == 0)
        GenUniformValues(pub);
    else if (pubValDis == 1)
        GenZiefValues(pub);
    return pub;
}

void generator::GenUniformAtts(Pub &pub)
{
    vector<int> a;
    for (int i = 0; i < pub.size; i++)
    {
        int x = random(attDom);
        while (CheckExist(a,x))
            x = random(attDom);
        a.push_back(x);
        Pair tmp;
        tmp.att = x;
        pub.pairs.push_back(tmp);
    }
}

void generator::GenZipfAtts(Pub &pub)
{
    first = true;
    vector<int> a;
    for (int i = 0; i < pub.size; i++)
    {
        int x = zipfDistribution(attDom, pubAttalpha);
        while (CheckExist(a,x))
            x = zipfDistribution(attDom, pubAttalpha);
        a.push_back(x);
        Pair tmp;
        tmp.att = x;
        pub.pairs.push_back(tmp);
    }
}


void generator::GenUniformValues(Pub &pub)
{
    for (int i = 0; i < pub.size; i++)
    {
        int x = random(valDom);
        pub.pairs[i].value = x;
    }
}

void generator::GenZiefValues(Pub &pub)
{
    first = true;
    for (int i = 0; i < pub.size; i++)
    {
        int x = zipfDistribution(valDom, pubValalpha);
        pub.pairs[i].value = x;
    }
}






//*********************intervalGenerator*******************************



void intervalGenerator::GenSubList()
{
    for (int i = 0; i < subs; i++)
    {
        IntervalSub sub = GenOneSub(i);

        subList.push_back(sub);
    }

    ofstream fileStream;
    fileStream.open("./Detail/subList.txt");
    for (int i=0; i<subs; ++i)
    {
        IntervalSub &sub = subList[i];
        fileStream << sub.id << "\t" << sub.size << "\t";
        for (int j=0; j<sub.size; ++j)
            fileStream << sub.constraints[j].att << "\t" << sub.constraints[j].lowValue << "\t" << sub.constraints[j].highValue << "\t";
        fileStream << "\n";
    }
    fileStream.close();
}

void intervalGenerator::ReadSubList(){
	ifstream fileStream; //read files
	fileStream.open("./Detail/subList.txt");
	for (int i=0; i<subs; ++i){
		IntervalSub sub;
		fileStream >> sub.id >> sub.size;
		for (int j=0; j<sub.size; ++j){
            IntervalCnt tmp;
			fileStream >> tmp.att >> tmp.lowValue >> tmp.highValue;
			sub.constraints.push_back(tmp);
		}
		subList.push_back(sub);
	}
	fileStream.close();
}
		
		

IntervalSub intervalGenerator::GenOneSub(int id)
{
    IntervalSub sub;
    sub.id = id;
//    sub.size = random(k) + 1;   // 1-k 个谓词
    sub.size =random(6)+5; // 5-10
    // 根据属性分布生成非空属性集
    if (subAttDis == 0)
        GenUniformAtts(sub);
    else if (subAttDis==1)
        GenZipfAtts(sub);

    // 在非空属性集上生成相应分布的属性值
    if (subValDis == 0)
        GenUniformValues(sub);
    else if (subValDis == 1)
        GenZiefValues(sub);
    return sub;
}

void intervalGenerator::GenUniformAtts(IntervalSub &sub)
{
    vector<int> a;
    for (int i=0; i<sub.size; ++i)
    {
        int x = random(attDom);
        while (CheckExist(a,x))
            x = random(attDom);
		a.push_back(x);
        IntervalCnt tmp;
        tmp.att = x;
        sub.constraints.push_back(tmp);
    }
}

void intervalGenerator::GenZipfAtts(IntervalSub &sub)
{
    vector<int> a;
    first = true;
    for (int i = 0; i < sub.size; i++)
    {
        int x = zipfDistribution(attDom, subAttalpha);
        while (CheckExist(a,x))
            x = zipfDistribution(attDom, subAttalpha);
        a.push_back(x);
        IntervalCnt tmp;
        tmp.att = x;
        sub.constraints.push_back(tmp);
    }
}

void intervalGenerator::GenUniformValues(IntervalSub &sub)
{
    for (int i = 0; i < sub.size; i++)
    {
	//width = 0.4 * (rand() / (RAND_MAX + 1.0)) +0.1; //0.1-0.5 random
        //width = 0.9 * (rand() / (RAND_MAX + 1.0)) + 0.1;
        width = rand() / (RAND_MAX + 1.0)*0.5;
        if (width<1){
            int x = random( int(valDom * (1.0 - width)) );
            int y = x + int(valDom * width);
            sub.constraints[i].lowValue = x;
            sub.constraints[i].highValue = y;
        }
        else {
            int x = random(valDom);
            int y = random(valDom);
            if (x<y)
            {
                sub.constraints[i].lowValue = x;
                sub.constraints[i].highValue = y;
            }
            else{
                sub.constraints[i].lowValue = y;
                sub.constraints[i].highValue = x;
            }
        }

    }
}

void intervalGenerator::GenZiefValues(IntervalSub &sub)
{
    first = true;
    int Dom = int(valDom * (1.0 - width));
    for (int i = 0; i < sub.size; i++)
    {
        int x = zipfDistribution(Dom, subValalpha);
        int y = x + int(valDom * width);
        sub.constraints[i].lowValue = x;
        sub.constraints[i].highValue = y;
    }
}


void intervalGenerator::GenPubList()
{
	c=0;
	first=true;
    for (int i = 0; i < pubs; i++)
    {	
		nowpub = i; // 现在要产生的事件的编号
		if(i == nexthot){
			
			Pub hot = GenHotPub(); // 实则就是一个随机事件(作为热点)
			hot.hotnum = hotsum;   // 这是第几个热点，从0开始
			hotlist.push_back(hot);

            int halflife = random(200) + 600;
            hotscale.push_back(random(200) + 600);
            nexthot += random(200) + 600;

			//int halflife=random(1000)+1500; //1500-2500 half-life time  热点持续时间？事件个数
			//hotscale.push_back(random(200)+100); //100-300 reduce time
			//nexthot += random(400) + 800; //800-1200 step to create new hotspot，计算下一个热点的位置i
            hotlife.push_back(halflife + i);
            ++hotsum;
            //nexthot = 1000;
            pubList.push_back(hot);
			cout<<"Hot" << hotsum << ' ' << i << ' ' << halflife + i << endl;
		}
		else{
			Pub pub = GenOnePub();
			pubList.push_back(pub);
		}
    }
	cout<<"Total generated "<<hotsum<<" hotspots\n\n"<<endl;// 热点个数
    ofstream fileStream;
    fileStream.open("./Detail/pubList.txt");
    for (int i = 0; i < pubs; i++)
    {
        Pub &pub = pubList[i];
        //fileStream << i << "\t";
        for (int j=0; j<pub.size; ++j)
            fileStream << pub.pairs[j].att << "\t" << pub.pairs[j].value << "\t";
        fileStream << pub.hotnum << "\n";
    }
    fileStream.close();
}

void intervalGenerator::GenPubList2()
{
    int beginHot = 1;       // 第1000个事件后产生第一个热点
    int intervalNum = 1;    // 热点消亡后间隔多久产生第二个热点
    bool pubState = 0;         // 0表示一般事件，1表示应该产生热点事件
    int maxHotNum = 1000;      // 一个热点最多持续多少个事件
    int nowHotNum = 0;         // 当前热点实际有多少个事件
    int nowHotNo = 0;          // 围绕当前热点已经产生的事件个数
    int maxHotAttNum=attDom;   // 最多有多少个维度产生波动
    int nowHotAttNum = attDom; // 当前有多少个维度产生波动
    double flut = 0.1;        // 产生波动的比例
    vector<Pub> hotPubList;    // 围绕当前热点产生的事件列表，先一次性产生，然后一个个取
    vector<bool> attHot;        // 当前热点产生波动的那些维度号
    
    nexthot = beginHot;

    for (int i = 1; i <= pubs; i++)
    {
        if (pubState) { // 产生热点事件
            if (nowHotNum == nowHotNo) {       // 当前热点的事件已经用完, 应该产生下一个热点了
                nowHotNo = 0;
                pubState = 0;                  // 接下来是一段产生一般事件的间隔期
                nexthot = i - 1 + intervalNum; // 间隔期的中止位置
                hotsum++;

                nowHotAttNum = maxHotAttNum / 2;
                attHot.clear();
                attHot.resize(attDom);
                for (int j = 0; j < nowHotAttNum; j++) {
                    int x = random(attDom);
                    while (attHot[x])
                        x = random(attDom);
                    attHot[x]=true;
                }

                nowHotNum = maxHotNum;         // 计算下一个热点持续多少个事件
                hotPubList.resize(nowHotNum);
                for (int q = 0; q < nowHotNum; q++) {
                    hotPubList[q].hotnum = hotsum;
                    hotPubList[q].size = attDom;
                    hotPubList[q].pairs.resize(attDom);
                }
                for (int j = 0; j < attDom; j++) {
                    // 生成这个维度上的每个事件的值
                    if (!attHot[j]) {
                        int valueAttJ = random(valDom);
                        for (int q = 0; q < nowHotNum; q++) {
                            hotPubList[q].pairs[j].att = j;
                            hotPubList[q].pairs[j].value = valueAttJ;
                        }
                    }
                    else {
                        int beginValue = random(valDom);
                        double range = flut * valDom;
                        int sign = random(2);
                        vector<int> valueList(nowHotNum);
                        for (int q = 0; q < nowHotNum; q++) {
                            valueList[q] = max(0, min(beginValue + random(range), valDom - 1));
                        }
                        sort(valueList.begin(), valueList.end());
                        for (int q = 0; q < nowHotNum; q++) {
                            hotPubList[q].pairs[j].att =j;
                            if (sign)
                                hotPubList[q].pairs[j].value = valueList[q];
                            else hotPubList[q].pairs[j].value = valueList[nowHotNum - q - 1];
                        }
                    }
                }
                cout <<"Hot " << hotsum << ": " << nexthot << ' ' << nowHotNum + nexthot << endl;
                i--;
                continue; // 这次没有产生事件
            }
            pubList.push_back(hotPubList[nowHotNo++]);
        }
        else {  // 产生一般事件
            pubList.push_back(GenOneUniformPub());
            if (i== nexthot) {
                pubState = 1; // 下一次产生热点事件
            }
        }
    }
    cout << "Total generated " << hotsum << " hotspots\n\n" << endl;// 热点个数
    ofstream fileStream;
    fileStream.open("./Detail/pubList.txt");
    for (int i = 0; i < pubs; i++)
    {
        Pub& pub = pubList[i];
        //fileStream << i << "\t";
        for (int j = 0; j < pub.size; ++j)
            fileStream << pub.pairs[j].att << "\t" << pub.pairs[j].value << "\t";
        fileStream << pub.hotnum << "\n";
    }
    fileStream.close();
}

void intervalGenerator::ReadPubList(){
	ifstream fileStream;
	fileStream.open("./Detail/pubList.txt");
	for (int i=0; i<pubs; ++i){
		Pub pub;
		pub.size = m;
		for (int j=0; j<pub.size; ++j){
			Pair tmp;
			fileStream >> tmp.att >> tmp.value;
			pub.pairs.push_back(tmp);
		}
		fileStream >> pub.hotnum;
		pubList.push_back(pub);
	}
	fileStream.close();
}

Pub intervalGenerator::GenHotPub()
{
    Pub pub;
    pub.size = m;
    if (pubAttDis == 0)
        GenUniformAtts(pub);
    else if (pubAttDis == 1)
        GenZipfAtts(pub);
    if (true)
        GenHotValues(pub);
    else if (pubValDis == 1)
        GenZiefValues(pub);
    return pub;
}

// Swhua: 随机产生一个事件--可以模拟一般情况和热点消失情况
Pub intervalGenerator::GenOneUniformPub() {
    Pub pub;
    pub.size = m;
    pub.hotnum = 0; // 0号表示没有热点
    GenUniformAtts(pub);
    for (int i = 0; i < m; i++)
    {
        int x = random(valDom);
        pub.pairs[i].value = x;
    }
    return pub;
}

Pub intervalGenerator::GenOnePub()
{
    Pub pub;
    pub.size = m;
    if (pubAttDis == 0)  // pubAttDis一定是0
        GenUniformAtts(pub);
    else if (pubAttDis == 1)
        GenZipfAtts(pub); // 这个函数没用, 因为已经把m赋给了pub.size
    if (pubValDis == 0) // 只有一个热点的情况
        GenUniformValues(pub);
    else if (pubValDis == 1) // 根据Zief分布找一个热点，产生热点事件值
        GenZiefValues(pub);
    return pub;
}

void intervalGenerator::GenUniformAtts(Pub &pub)
{
    for (int i = 0; i < pub.size; i++) // 即每个维度都有值，非空
    {
        Pair tmp;  //the tmp is the pair of attribute and value.
        tmp.att = i;
		tmp.value = 0;
        pub.pairs.push_back(tmp);
    }
}

void intervalGenerator::GenZipfAtts(Pub &pub)
{
    first = true;
    vector<int> a;
    for (int i = 0; i < pub.size; i++)
    {
        int x = zipfDistribution(attDom, pubAttalpha);
        while (CheckExist(a,x))
            x = zipfDistribution(attDom, pubAttalpha);
        a.push_back(x);
        Pair tmp;
        tmp.att = x;
        pub.pairs.push_back(tmp);
    }
}

void intervalGenerator::GenHotValues(Pub &pub)
{
    for (int i = 0; i < pub.size; i++)
    {
        int x = random(valDom);
        pub.pairs[i].value = x;
    }
}

void intervalGenerator::GenUniformValues(Pub &pub)
{
	int changesize = random(pub.size+1); // 随机修改这么多个维度上热点事件的值
	vector<int> a;                       // 记录哪些维度上要修改，保证维度不同，不修改的就是0，
    for (int i = 0; i < changesize; i++)
    {
        int x = random(attDom);
        while (CheckExist(a,x))
            x = random(attDom);
		a.push_back(x);
        // 在这些维度上偏离热点属性值多少距离
        pub.pairs[x].value = random(valDom/10)-valDom/20; //range of away from hot event: +-5%
        //pub.pairs[x].value = random(valDom / 50) - valDom / 100;
    }
    int hotid = 0;
    if (hotlist.size() > 1)
        hotid = 1;
    pub.hotnum = hotid;
	for (int i = 0; i < pub.size; i++){
        // 偏离加上热点，只有一个热点，即0号热点
		pub.pairs[i].value += hotlist[hotid].pairs[i].value; 
		if (pub.pairs[i].value < 0) pub.pairs[i].value = 0;
		if (pub.pairs[i].value >= valDom) pub.pairs[i].value = valDom-1;
	}

}

void intervalGenerator::GenZiefValues(Pub &pub)
{
    for (int i = 0; i < pub.size; i++)
    {
        int x = zipfDistribution(valDom/20, pubValalpha);     // 0~5% pubValalpha是1
		int sign = random(2);                                 // 随机取正负号
        if(sign) pub.pairs[i].value = x;                      
		else pub.pairs[i].value = -x;  
    }
	vector<double> hotposs; 
	double posssum = 0, tmpsum = 0, nowposs;
	int result = 0;
	for(int i = 0; i < hotsum; ++i){ // nowpub: 现在要产生的事件的编号id
		double poss = 1.0/(1+exp(10*(nowpub-hotlife[i])/hotscale[i])); //sigmod-like function
		hotposs.push_back(poss);
		posssum += poss;
	}
	nowposs = posssum * (rand() / (RAND_MAX + 1.0));
	for(int i = 0; i < hotsum; ++i){
		tmpsum += hotposs[i];
		if(nowposs < tmpsum){
			result = i;         // 找到nowposs这个概率所对应的热点的id
			break;
		}
		if(i == hotsum-1){
			cout<<"generate poss error!"<<endl; // ? 为什么不能等于？
		}
	}
	Pub &hot = hotlist[result];  // 取出当前打算产生的热点事件
	pub.hotnum = result;         // 围绕这个热点事件产生一些波动
	for (int i = 0; i < pub.size; i++){
		pub.pairs[i].value += hot.pairs[i].value; // 加上这个热点
		if (pub.pairs[i].value < 0) pub.pairs[i].value = 0;
		if (pub.pairs[i].value >= valDom) pub.pairs[i].value = valDom-1;
	}
}

int generator::zipfDistribution(int n, double alpha)
{
    // alpha > 0
    double z;                     // Uniform random number (0 < z < 1)
    double sum_prob;              // Sum of probabilities
    double zipf_value = 0;            // Computed exponential value to be returned
    int i;                     // Loop counter

    if (first)
    {
        for (i = 1; i <= n; i++)
            c += (1.0 / pow((double) i, alpha));
        c = 1.0 / c;
        first = false;
    }

    while (true)
    {
        // Pull a uniform random number (0 < z < 1)
        do
        {
            z = (double) rand() / RAND_MAX;
        } while ((z == 0) || (z == 1));

        // Map z to the value
        sum_prob = 0;
        for (i = 1; i <= n; i++)
        {
            sum_prob = sum_prob + c / pow((double) i, alpha);
            if (sum_prob >= z)
            {
                zipf_value = i;
                break;
            }
        }
        if (zipf_value >= 1 && zipf_value <= n)
            return int(zipf_value - 1);
    }
}

bool generator::CheckExist(vector<int> a,int x)
{
    for (int i = 0; i < a.size(); i++)
        if (a[i] == x)
            return true;
    return false;
}

int intervalGenerator::zipfDistribution(int n, double alpha)
{
    double z;                     // Uniform random number (0 < z < 1)
    double sum_prob;              // Sum of probabilities
    int zipf_value=0;            // Computed exponential value to be returned
    int    i;                     // Loop counter
    if (first)   //初始化
    {	
		c=0;
        for (i=1; i<=n; i++)
            c += 1.0 / pow((double) i, alpha);
        c = 1.0 / c;
        first = false;
    }

    while(true)
    {
        // Pull a uniform random number (0 < z < 1) z可以等于0吧？
        do {
            z = (double) rand() / RAND_MAX;
        } while ((z == 0) || (z == 1));

        // Map z to the value
        sum_prob = 0;
        for (i = 1; i <= n; i++)
        {
			//cout<<z<<' '<<i<<endl;
            sum_prob += c / pow((double) i, alpha);
            if (sum_prob >= z) 
            {
                zipf_value = i;
                break;
            }
        }

        //cout << zipf_value << endl;
        if (zipf_value>=1&&zipf_value<=n)
            return (zipf_value-1);

    }
}

bool intervalGenerator::CheckExist(vector<int> a,int x)
{
    for (int i = 0; i < a.size(); i++)    
        if (a[i] == x)                                   //Why do we check whether  a[i]=x?
            return true;
    return false;
}

int generator::random(int x)
{
    return (int) (x * (rand() / (RAND_MAX + 1.0)));     //generate [0,x]
}

int intervalGenerator::random(int x)
{
    return (int) (x * (rand() / (RAND_MAX + 1.0)));
}

/*
void intervalGenerator::GenzipfPubList() {
    ofstream pubDetail;
    pubDetail.open("pubDetail.txt");
    pubDetail << "pubID" << "\tpair\n";

    //srand(time(NULL));

    vector<int> attsVec;
    vector<int> valuesVec;

    first = true;
    c = 0;

    for (int i = 0; i < pubs; i++) {
        vector<int> a;
        for (int j = 0; j < m; j++) {
            int x = zipfDistribution(atts, alpha);
            while (CheckExist(a, x))
                x = zipfDistribution(atts, alpha);
            a.push_back(x);
            attsVec.push_back(29-x);
        }
    }

    //cout << "check1" << endl;
    first = true;
    c = 0;
    int limit = pubs * m;
    for (int i=0; i<limit; i++){
        valuesVec.push_back(zipfDistribution(valDom, alpha));
    }

    //cout << "check2" << endl;

    int k = 0;

    for (int i = 0; i < pubs; i++) {
        Pub pub;
        pub.size = m;
        for (int j = 0; j < m; j++) {
            Pair tmp;
            tmp.att = attsVec[k];
            tmp.value = (valuesVec[k]+400000)%valDom;
            pub.pairs.push_back(tmp);
            ++k;
        }
        pubList.push_back(pub);
    }

    for (int i = 0; i < pubs; i++)
    {
        pubDetail << i << "\t";
        for (size_t j=0; j< m; ++j)
        {
            pubDetail << pubList[i].pairs[j].att << "=" << pubList[i].pairs[j].value << " ";
        }
        pubDetail << "\n";
    }

    pubDetail.close();
}

void intervalGenerator::GenUniformSubList() {
    ofstream subDetail;
    subDetail.open("subList.txt");
    subDetail << "subID" << "\tcons\n";

    vector<int> attsVec;
    vector<int> valuesVec;

    for (int i = 0; i < subs; i++) {
        vector<int> a;
        for (int j = 0; j < cons; j++) {
            int x = random(atts);
            while (CheckExist(a, x))
                x = random(atts);
            a.push_back(x);
            attsVec.push_back(x);
        }
    }

    int limit = subs * cons;
    for (int i=0; i<limit; i++)
        valuesVec.push_back(random( int(valDom * (1.0 - width)) ));


    int k=0;
    for (int i = 0; i < subs; i++) {
        IntervalSub sub;
        sub.id = i;
        sub.size = cons;
        for (int j = 0; j < cons; j++) {
            IntervalCnt tmp;
            tmp.att = attsVec[k];
            tmp.lowValue = valuesVec[k];
            tmp.highValue = valuesVec[k] + int(valDom * width);
            sub.constraints.push_back(tmp);
            ++k;
        }
        subList.push_back(sub);
    }

    for (int i = 0; i < subs; i++)
    {
        subDetail << subList[i].id << "\t";
        for (size_t j=0; j<subList[i].constraints.size(); ++j)
            subDetail << subList[i].constraints[j].att << ":" << subList[i].constraints[j].lowValue << "-" << subList[i].constraints[j].highValue << " ";
        subDetail << "\n";
    }
    subDetail << flush;
    subDetail.close();

}
*/
