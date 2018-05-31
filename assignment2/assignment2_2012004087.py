import numpy as np
import pandas as pd
import math


def Info (lst):
    ret = .0
    w_count = {}
    start_idx = 0
    end_idx = 0
    lst_T = [list(i) for i in zip(*lst)]
    for item in lst_T[0]:
        try:
            w_count[item] += 1
        except:
            w_count[item] = 1

    for val in w_count.values():
        ret -= val/len(lst) * math.log(val/len(lst) ,2)
    return ret

def Info_A (lst):

    ret = .0
    w_count = {}
    start_idx = 0
    end_idx = 0
    lst.sort(key = lambda x : (x[1], x[0]))
    lst_T = [list(i) for i in zip(*lst)]
    for item in lst_T[1]:
        try:
            w_count[item] += 1
        except:
            w_count[item] = 1
    for val in w_count.values() :
        end_idx += val
        ret += val / len(lst) * Info(lst[start_idx : end_idx])
        start_idx = end_idx

    print("엔트로피 : " +str(ret))

def loadWord () :

#    fileName = sys.argv[1]
    fileName = "WordEmbedding.txt"
    try:
        fp = open(fileName, 'r')
    except:
        print("파일을 열 수 없습니다. 다시 시도해주세요.\n")

    list = []

    while True:
        templist = []
        line = fp.readline()
        if not line:  # 파일의 내용을 한줄씩 읽고 다음줄이 없으면 while문 break
            break
        templist.append(line.split()[0])
        line = fp.readline()
        if not line:  # 파일의 내용을 한줄씩 읽고 다음줄이 없으면 while문 break
            break
        templist += [line.split(',')]
        for i in range(0, 300):
            templist[1][i] = float(templist[1][i])
        list.append(templist)
 #   list.sort(key = lambda x: (x[1], x[2]))

    return list

def loadWordTopic (DB) :
#    fileName = sys.argv[1]
    fileName = "WordTopic.txt"
    try:
        fp = open(fileName, 'r')
    except:
        print("파일을 열 수 없습니다. 다시 시도해주세요.\n")
    i = 0
    list = []
    while True:
        templist = []
        wordClass = fp.readline()
        wordClass = wordClass.split('[')[1]
        wordClass = wordClass.split(']')[0]

        while True:
            line = fp.readline()
            if line =='\n' :  # 파일의 내용을 한줄씩 읽고 다음줄이 없으면 while문 break
                break
            if not line:  # 파일의 내용을 한줄씩 읽고 다음줄이 없으면 while문 break
                return DB
            DB[i].append(wordClass)
            i += 1

def Cos_Sim (word1, word2) :
    word1_ProductSum = .0
    word2_ProductSum = .0
    product1and2 = .0

    for i in range(0, 300) :
        word1_ProductSum += math.pow(word1[1][i],2)
        word2_ProductSum += math.pow(word2[1][i],2)
        product1and2 += word1[1][i] * word2[1][i]

    word1_ProductSum = math.sqrt(word1_ProductSum)
    word2_ProductSum = math.sqrt(word2_ProductSum)

    return product1and2 / (word1_ProductSum * word2_ProductSum)

def SetAllCos_Sim (old_list) :
    print("2차원 행렬의 형태로 Cosine Similarity를 생성중입니다.")
    new_list = []
    row_names = []
    rank = []

    for i in range(0, len(old_list)-1) :
        for j in range(i+1, len(old_list)):
            temp_list = []
            temp_list.append(old_list[i][0])
            temp_list.append(old_list[j][0])
            temp_list.append(Cos_Sim(old_list[i], old_list[j]))
            rank.append(temp_list)

    rank.sort(key=lambda x : x[2])

    for i in range(0, len(old_list)) :
        row_names.append(old_list[i][0])
        for j in range(0, len(old_list)):
            if i == j :
                new_list.append(None)
            else :
                new_list.append(Cos_Sim(old_list[i], old_list[j]))

    matrix = np.reshape(new_list, (len(old_list), len(old_list)))
    df = pd.DataFrame(matrix, columns=row_names, index=row_names)
    return [rank, df]

def Eu_Sim (word1, word2) :
    word_ProductSum = .0

    for i in range(0, 300) :
        word_ProductSum += math.pow(word1[1][i] - word2[1][i],2)

    return math.sqrt(word_ProductSum)

def SetAllEu_Sim (old_list) :
    print("2차원 행렬의 형태로 Euclidian Similarity를 생성중입니다.")
    new_list = []
    row_names = []
    rank = []

    for i in range(0, len(old_list)-1) :
        for j in range(i+1, len(old_list)):
            temp_list = []
            temp_list.append(old_list[i][0])
            temp_list.append(old_list[j][0])
            temp_list.append(1 - Eu_Sim(old_list[i], old_list[j]) / (2*math.sqrt(300)))

            rank.append(temp_list)

    rank.sort(key=lambda x : x[2])

    for i in range(0, len(old_list)) :
        row_names.append(old_list[i][0])
        for j in range(0, len(old_list)):
            if i == j :
                new_list.append(None)
            else :
                new_list.append(1 - Eu_Sim(old_list[i], old_list[j]) / (2*math.sqrt(300)))


    matrix = np.reshape(new_list, (len(old_list), len(old_list)))
    df = pd.DataFrame(matrix, columns=row_names, index=row_names)

    return [rank, df]

def Clustering (rankAndDf, threshold) :
    print("clustering 생성 중")
    return_list = []

    cur_idx = 0
    cur_threshold = threshold[cur_idx]

    rank = rankAndDf[0]
    df = rankAndDf[1]

    while len(rank) != 0 :
        temp = rank.pop()
        word1 = temp[0]
        word2 = temp[1]
        for i in df.index :
            if word1 in i :
                word1 = i
                break
        for i in df.columns:
            if word2 in i:
                word2 = i
                break
        if temp[2]< cur_threshold :
            return_list.append(df.index)
            cur_idx += 1
            if cur_idx == len(threshold):
                return return_list

            cur_threshold = threshold[cur_idx]

        if df[word1][word2] is None :
            continue
        if df[word1][word2] == temp[2]:

            temp_df1 = df.pop(word1)
            temp_df2 = df.pop(word2)
            temp_df1.name += "+" + temp_df2.name
            for i in temp_df1.index:
                if temp_df1[i] is None:
                    continue
                elif temp_df2[i] is None:
                    temp_df1[i] = None
                elif temp_df2[i] < temp_df1[i]:
                    temp_df1[i] = temp_df2[i]

            df = pd.concat([df, temp_df1], axis=1, join_axes=[df.index])
            temp_df1 = df.loc[word1]
            temp_df2 = df.loc[word2]

            temp_df1.name += "+" + temp_df2.name
            for i in temp_df1.index:

                if temp_df1[i] is None:
                    continue
                elif temp_df2[i] is None:
                    temp_df1[i] = None
                elif temp_df2[i] < temp_df1[i]:
                    temp_df1[i] = temp_df2[i]

            df = df.drop([word1, word2])
            df = df.append(temp_df1)

    return df.index

def SetClusterNumber (DB,Cluster):
    for i in range(0, len(DB)) :
        for j in range(0, len(Cluster)):
            if DB[i][0] in Cluster[j] :
                DB[i].append(j)
                break
    return DB

def Silhouette (DB):
    ret = .0
    inner_dis = 0
    w_count = {}

    count_a = 0
    count_b = 0
    newDB = DB[:]
    newDB.sort(key=lambda x: x[3])
    DB_T = [list(i) for i in zip(*newDB)]

    silhouette = .0
    for item in DB_T[3]:
        try:
            w_count[item] += 1
        except:
            w_count[item] = 1

    for i in range(0, len(DB)):
        start_idx = 0
        end_idx = 0
        a_val = .0
        b_val = 10000.0


        for val in w_count.values():

            sum = .0
            idx = 0
            word_ProductSum = .0
            end_idx += val
            while idx != val:

                for j in range(0, 300):
                    word_ProductSum += math.pow(newDB[start_idx + idx][1][j] - newDB[i][1][j], 2)
                sum += math.sqrt(word_ProductSum)
                idx += 1

            if start_idx <= i and i < end_idx :
                a_val = sum / val
            elif b_val > (sum / val):
                b_val = sum / val
            start_idx += val

        if b_val < a_val :
            count_a += 1
            silhouette += (b_val / a_val - 1.0)
        elif b_val == a_val :
            silhouette += .0
        else :
            count_b +=1
            silhouette += (1.0 - a_val / b_val)

    print("silhouette : " + str(silhouette / len(DB)))
   # print("the number of a > b" + str(count_a)+ " The number of a < b " + str(count_b))
DB = loadWord()

rankAndDf = SetAllCos_Sim(DB)
EuramlAmdDf = SetAllEu_Sim(DB)
#print(EuramlAmdDf[0][0:10])
#print(EuramlAmdDf[0][-11:-1])
cos_threshold = [0.8, 0.6, 0.4 , 0.2]
eu_threshold = [0.89, 0.88, 0.86, 0.85]
cluster_cos_list = Clustering(rankAndDf, cos_threshold)
cluster_eu_list =  Clustering(EuramlAmdDf, eu_threshold)

print("cossine")
for i in range(0,4):
    DB = loadWord()
    DB = loadWordTopic(DB)
    DB = SetClusterNumber(DB, cluster_cos_list[i])
    transPose = [list(j) for j in zip(*DB)]
    new_lst = [transPose[2],transPose[3]]
    new_lst = [list(j) for j in zip(*new_lst)]
    print(cos_threshold[i])
#    print(Info(new_lst)- Info_A(new_lst))
    Info_A(new_lst)
    Silhouette(DB)
    if i == 2:

        output = "WordClustering.txt"
        try:
            fp = open(output, 'w')
        except:
            print("파일을 쓸 수 없1습니다. 다시 시도해주세요.\n")

        for item in DB:
            fp.write(item[0] + "\n")
            for item2 in item[1][:-1]:
                fp.write(str(item2))
                fp.write(",")
            fp.write(str(item[1][-1]) + "\n")
            fp.write(str(item[3]) + "\n")

print("euclidian")
for i in range(0,4):
    DB = loadWord()
    DB = loadWordTopic(DB)
    DB = SetClusterNumber(DB, cluster_eu_list[i])
    transPose = [list(j) for j in zip(*DB)]
    new_lst = [transPose[2],transPose[3]]
    new_lst = [list(j) for j in zip(*new_lst)]
    print(eu_threshold[i])
#    print(Info(new_lst)- Info_A(new_lst))
    Info_A(new_lst)
    Silhouette(DB)
