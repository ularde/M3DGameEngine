local call_count = 0

function Main_OnInit()
    --LoadScene("Scenes\\example3.mmap")
end

function Main_OnTickFinish()
    if (GetKey(49) == 1) then
        LoadScene("scenes\\example1.mmap")
    elseif (GetKey(50) == 1) then
        LoadScene("scenes\\example2.mmap")
    elseif (GetKey(51) == 1) then
        LoadScene("scenes\\example3.mmap")
    elseif (GetKey(52) == 1) then
        LoadScene("scenes\\example4.mmap")
    end
end

function Main_OnDestroy()
    
end