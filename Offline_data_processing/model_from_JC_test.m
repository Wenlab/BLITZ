%%The model here is from https://www.nature.com/articles/s41592-020-0759-2

headingAngle_rad=deg2rad(headingAngle);
headingAngle_rad_unwrap=unwrap(headingAngle_rad);

h = [1/2 1/2];
binomialCoeff = conv(h,h);
for n = 1:4
    binomialCoeff = conv(binomialCoeff,h);
end
binomialMA = filter(binomialCoeff, 1, headingAngle_rad_unwrap);

test=zeros(size(sum_curv,1),18);
bout_idx=[];
FLAG=0;
for n=2:size(sum_curv,1)

    test(n,1)=binomialMA(n)-binomialMA(n-1);
    test(n,2)=sqrt(((head(n,:)-head(n-1,:)).*(head(n,:)-head(n-1,:))*[1;1]));
    if sum_curv(n)*sum_curv(n-1)<=0
        test(n,3)=1;
    end
    if abs(sum_curv(n))>0.4&&(n-FLAG)>14
        bout_idx=[bout_idx;n-2;n-1;n;n+1;n+2;n+3;n+4;n+5;n+6;n+7;n+8;n+9;n+10;n+11;n+12;n+13];
        %%bout_idx=[bout_idx;n-2;n-1;n;n+1;n+2;n+3;n+4;n+5;n+6;n+7];
        FLAG=n;
    end
end



m=4;


for i=m:size(sum_curv,1)
    for n=0:m-1
        test(i,5)=test(i,5)+(0.9^n)*abs(sum_curv(i-n));
        test(i,6)=test(i,6)+(0.9^n)*sum_curv(i-n);
    end
end
m=5;
for i=m:size(sum_curv,1)
    for n=0:m-1
        test(i,7)=test(i,5)+(0.9^n)*abs(sum_curv(i-n));
        test(i,8)=test(i,6)+(0.9^n)*sum_curv(i-n);
    end
end
mdl = fitlm(test(bout_idx,5),test(bout_idx,2));
mdl
plot(mdl);
