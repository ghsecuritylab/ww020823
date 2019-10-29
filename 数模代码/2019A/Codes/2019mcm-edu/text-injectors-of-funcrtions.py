import numpy as np
import matplotlib.pyplot as plt
def functions_d_V():
    time_stamp=[]
    V=[0.798777805210716,1.59472540564190,2.38913898703494,3.18331473513124,3.97854883567218,4.77613747439915,5.57737683705355,6.38356310937677,7.19599247711021,8.01596112599524,8.84476524177327,9.68370101018568,10.5340646169739,11.3971522478792,12.2742600886432,13.1666843250070,14.0757211427123,15.0026667275002,15.9488172651123,16.9154689412899,17.9039179417745,18.8923669422590,19.8808159427435,20.8692649432281,21.8577139437126,22.8461629441971,23.8346119446816,24.8230609451662,25.8115099456507,26.7999589461352,27.7884079466198,28.7768569471043,29.7653059475888,30.7537549480734,31.7422039485579,32.7306529490424,33.7191019495269,34.7075509500115,35.6959999504960,36.6844489509805,37.6728979514651,38.6613469519496,39.6497959524341,40.6382449529186,41.6266939534032,42.6151429538877,43.6035919543722,44.5920409548568,45.5804899553413,46.5689389558258,47.5573879563103,48.5458369567949,49.5342859572794,50.5227349577639,51.5111839582485,52.4996329587330,53.4880819592175,54.4765309597020,55.4649799601866,56.4534289606711,57.4418779611556,58.4303269616402,59.4187759621247,60.4072249626092,61.3956739630938,62.3841229635783,63.3725719640628,64.3610209645473,65.3494699650319,66.3379189655164,67.3263679660009,68.3148169664854,69.3032659669700,70.2917149674545,71.2801639679390,72.2686129684235,73.2570619689080,74.2455109693926,75.2339599698771,76.2224089703616,77.2108579708461,78.1993069713306,79.1877559718152,80.1762049722997,81.1646539727842,82.1531029732687,83.1415519737533,84.1300009742378,85.1184499747223,86.1068989752068,87.0953479756913,88.0837969761759,89.0722459766604,90.0606949771449,91.0491439776294,92.0375929781140,93.0260419785985,94.0144909790830,95.0029399795675,95.9913889800520,96.9798379805366,97.9682869810211,98.9567359815056,99.9451849819901,100.933633982475,101.922082982959,102.910531983444,103.898980983928,104.887429984413,105.875878984897,106.864327985382,107.852776985866,108.841225986351,109.829674986835,110.818123987320,111.806572987804,112.795021988289,113.783470988773,114.771919989258,115.760368989742,116.748817990227,117.737266990712,118.725715991196,119.714164991681,120.702613992165,121.691062992650,122.679511993134,123.667960993619,124.656409994103,125.644858994588,126.633307995072,127.621756995557,128.610205996041,129.598654996526,130.587103997010,131.575552997495,132.564001997979,133.552450998464,134.540899998948,135.529348999433,136.517797999918,137.506247000402,138.494696000887,139.483145001371,140.471594001856,141.460043002340,142.448492002825,143.436941003309,144.425390003794,145.413839004278,146.402288004763,147.390737005247,148.379186005732,149.367635006217,150.356084006701,151.344533007186,152.332982007670,153.321431008155,154.309880008639,155.298329009124,156.286778009608,157.275227010093,158.263676010577,159.252125011062,160.240574011546,161.229023012031,162.217472012516,163.205921013000,164.194370013485,165.182819013969,166.171268014454,167.159717014938,168.148166015423,169.136615015907,170.125064016392,171.113513016876,172.101962017361,173.090411017845,174.078860018330,175.067309018814,176.055758019299,177.044207019784,178.032656020268,179.021105020753,180.009554021237,180.998003021722,181.986452022206,182.974901022691,183.963350023175,184.951799023660,185.940248024144,186.928697024629,187.917146025113,188.905595025598,189.894044026083,190.882493026567,191.870942027052,192.859391027536,193.847840028021,194.836289028505,195.824738028990,196.803339306851,197.772348898779,198.732022841464,199.682617171594,200.624387925861,201.557591140952,202.482482853558,203.399319100369,204.308355918074,205.209849343364,206.104055412926,206.991230163452,207.871629631631,208.745509854153,209.613126867706,210.474736708982,211.330595414669,212.180959021457,213.026083566036,213.866225085095,214.701639615325,215.532583193415,216.359311856054,217.182081639932,218.001148581739,218.816768718164,219.629198085897,220.438692721629,221.245508662047,222.049901943843,222.852128603705,223.652444678324,224.451106204389,225.248369218589,226.044489757615,226.839723858156,227.634327556902,228.428556890542,229.222667895766,230.016916609263,230.811559067724,231.606851307838,232.403049366295,233.200409279783,233.999187084994]
    d=[1.40000000000000,1.45500000000000,1.51000000000000,1.56500000000000,1.62000000000000,1.67500000000000,1.73000000000000,1.78500000000000,1.84000000000000,1.89500000000000,1.95000000000000,2.00500000000000,2.06000000000000,2.11500000000000,2.17000000000000,2.22500000000000,2.28000000000000,2.33500000000000,2.39000000000000,2.44500000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.50000000000000,2.47555555555556,2.45111111111111,2.42666666666667,2.40222222222222,2.37777777777778,2.35333333333333,2.32888888888889,2.30444444444444,2.28000000000000,2.25555555555556,2.23111111111111,2.20666666666667,2.18222222222222,2.15777777777778,2.13333333333333,2.10888888888889,2.08444444444444,2.06000000000000,2.03555555555556,2.01111111111111,1.98666666666667,1.96222222222222,1.93777777777778,1.91333333333333,1.88888888888889,1.86444444444444,1.84000000000000,1.81555555555556,1.79111111111111,1.76666666666667,1.74222222222222,1.71777777777778,1.69333333333333,1.66888888888889,1.64444444444444,1.62000000000000,1.59555555555556,1.57111111111111,1.54666666666667,1.52222222222222,1.49777777777778,1.47333333333333,1.44888888888889,1.42444444444444,1.40000000000000]
    for i in range(0,246):
        time_stamp.append(i/100)
    print(time_stamp)
    print(V)
    plt.figure()
    plt.plot(time_stamp,V,'g')
    plt.xlabel('time')
    plt.ylabel('Volumn')
    plt.show()
if __name__=="__main__":
    functions_d_V()