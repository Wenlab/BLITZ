%%sum=0;
path='D:\working\Data\automl\tailCenterline\20210105_1632_h2bg6f_9dpf_control_eyes\';
addpath(path);
file=dir(fullfile(path,'*.mat'));
filenames={file.name}';
for aa=1:length(filenames)
    filename=filenames{aa};
    load(filename);
end

test4findbout;
bout_start = false;
bout_idx=[];
bout_start_idx=-20;
for i=1:120000

    if abs(output(i,4))>6000 && (~bout_start)
        bout_idx=[bout_idx;i-2,i+13];
        bout_start_idx=i;
        bout_start=true;
    end
    if i==bout_start_idx+13
        bout_start=false;
    end
end

% % load(dir(fullfile(path,'*.mat')));
headingAngle_rad=deg2rad(headingAngle);
headingAngle_rad_unwrap=unwrap(headingAngle_rad);

h = [1/2 1/2];
binomialCoeff = conv(h,h);
for n = 1:4
    binomialCoeff = conv(binomialCoeff,h);
end
binomialMA = filter(binomialCoeff, 1, headingAngle_rad_unwrap);

% for exp=boutStructure
%     sum=sum+size(exp.BoutInf,1);
% end

% DataSet=zeros(size(bout_idx,1),13);
% sumbout=0;
% max=0;
% del_id=[];
boutDuration=bout_idx(:,2)-bout_idx(:,1)+1;
bout_idx_idx=find((boutDuration>4).*(boutDuration<15));
DataSet=zeros(size(bout_idx,1),18);
%%DataSet_idx=1;
%%del_id=[];
for i=1:size(bout_idx,1)
    DataSet(i,1:16)=output(bout_idx(i,1):bout_idx(i,2));
    DataSet(i,17)=output(bout_idx(i,2))-output(bout_idx(i,1));
    DataSet(i,18)=sqrt((double(output(bout_idx(i,2),1:2)-output(bout_idx(i,1),1:2)).*(output(bout_idx(i,2),1:2)-output(bout_idx(i,1),1:2))*[1;1]));
end

for bout=bout_idx_idx'
    DataSet(DataSet_idx,1:boutDuration(bout))=sum_curv(bout_idx(bout,1):bout_idx(bout,2));
    DataSet(DataSet_idx,15)=binomialMA(bout_idx(bout,2))-binomialMA(bout_idx(bout,1));

    DataSet(DataSet_idx,16)=sqrt(((head(bout_idx(bout,2),:)-head(bout_idx(bout,1),:)).*(head(bout_idx(bout,2),:)-head(bout_idx(bout,1),:))*[1;1]));
    if DataSet(DataSet_idx,16)<15||DataSet(DataSet_idx,16)>150
        del_id=[del_id;DataSet_idx];
    end

    DataSet_idx=DataSet_idx+1;

end

DataSet(del_id,:)=[];
save([path,'bout4ml.mat'],'DataSet');


%%writematrix(DataSet, [path,'bout4ml.mat']);
clear all;

for i=1:12000
    if DataSet(i,17)>180
        DataSet(i,1)=DataSet(i,1)-360;
    end
    if DataSet(i,1)<=(-1*pi)
        DataSet(i,1)=DataSet(i,1)+2*pi;
    end
end
for i=0:19
    for j=1:10040
        if totalDataSet1(j,1)>(-1*pi+i*pi/10)&&totalDataSet1(j,1)<=(-1*pi+(i+1)*pi/10)
        totalDataSet1(j,2)=i;
        end
    end
end

for i=1:120000
    if abs(output(i,4))>1000000
        output(i,4)=NaN;
    end
end
