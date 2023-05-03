import math

def distance(x1,y1,x2,y2):#(x1,y1)到(x2,y2)的距离
    return math.sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) )

def area(x1,y1,center):
    dis1=distance(x1+1,y1+1,center,center)#方格右下角到圆心的距离
    dis2=distance(x1,y1,center,center)#方格左上角到圆心的距离
    if(dis1>=center):
        return 0
    if(dis2<=center):
        return 1
    x=center-dis1
    if(x <= math.sqrt(2)/2):
        return x*x
    else:
        return 2*x*math.sqrt(2)-x*x-1

#以左上角为(0,0)建立坐标系
sideLength=int(input("输入正方形的边长："))
matrix = [[0 for col in range(sideLength)] for row in range(sideLength)]
center=sideLength/2
centerInt=int(center-0.5)#向下取整

for i in range(0, centerInt, 1) :#[0,center)即为m～n之间所有数
    matrix[i][centerInt]=1

for i in range(0, sideLength, 1) :
    matrix[centerInt][i]=1

for i in range(0, centerInt, 1) :
    for j in range(0, centerInt, 1):
        matrix[i][j]=area(i,j,center)

for i in range(0, centerInt, 1):
    for j in range(centerInt+1, sideLength, 1) :
        matrix[i][j]=matrix[i][sideLength-1-j]

for i in range(centerInt+1, sideLength, 1):
    for j in range(0, sideLength, 1) :
        matrix[i][j]=matrix[sideLength-1-i][j]

#矩阵输出
for i in range(sideLength):
    for j in range(sideLength):
        print('{:.6f}'.format(matrix[i][j]),end=" ")
    print()
