package assign7;

public class DictNode implements Comparable<DictNode>{
    String word, meaning, type;
    
    public DictNode()   //constructor
    {
        
    }
    
    public DictNode(String w, String m, String t)   //overloading
    {
        word = w;
        meaning = m;
        type = t;
    }
    
    @Override
     public int compareTo(DictNode o)
     {
         int wComp = word.compareToIgnoreCase(o.word);
         int mComp = meaning.compareToIgnoreCase(o.meaning);
         int tComp = type.compareToIgnoreCase(o.type);
         
         if(wComp!=0)
             return wComp;
         else if(mComp!=0)
             return mComp;
         else
             return tComp;
     }  
}
