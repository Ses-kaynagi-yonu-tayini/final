function degree = findPeaks3(Fx)
    
    peaks = zeros(1,5);
    indexies = zeros(1,5);
    for i = (1:6)
        [p,~]=findpeaks(Fx(10:end,i),'npeaks',3,"SortStr","descend");
        peaks(i) = sum(p);
    end
    [~,index] = max(peaks); 

    [maxFx,in] = max(Fx(:,index));

    fprintf("Baskýn mikrofon : %.0f\n",index);
    degree = (index-2) * 60;
    
    if index-1 == 0
        index = 6;
    else
        index = index -1;
    end
    
    mic_prev =  Fx(in,index);
 
    if index+2 >6
        index = mod(index+2,6);
    else
        index = index +2;
    end
    

    mic_next = Fx(in,index);

    t = mic_next + mic_prev;
    
    degree  = degree +  (120 * mic_prev / t);
    if degree < 360
        degree = degree + 360;
    end
    if degree >=360
        degree = mod(degree,360);    
    end
       
end