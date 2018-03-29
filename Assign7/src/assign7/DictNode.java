package assign7;

import java.util.Comparator;

public class DictNode {
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
    
    public static Comparator<DictNode> compareByAllFields = new Comparator<DictNode>()
    {
        @Override
        public int compare(DictNode a, DictNode b)
        {
            int wComp = a.word.compareToIgnoreCase(b.word);
            int mComp = a.meaning.compareToIgnoreCase(b.meaning);
            int tComp = a.type.compareToIgnoreCase(b.type);

            if(wComp!=0)
                return wComp;
            else if(tComp!=0)
                return tComp;
            else
                return mComp;        
        };
    };
    
    public static Comparator<DictNode> compareByWord = new Comparator<DictNode>()
    {
        @Override
        public int compare(DictNode a, DictNode b)
        {
            return (int) a.word.compareToIgnoreCase(b.word.trim().replaceAll("\\s+", " "));
        };
    };
    
    public int compareAllFields(DictNode o)
    {
        int wComp = word.compareToIgnoreCase(o.word);
        int mComp = meaning.compareToIgnoreCase(o.meaning);
        int tComp = type.compareToIgnoreCase(o.type);

        if(wComp!=0)
            return wComp;
        else if(tComp!=0)
            return tComp;
        else
            return mComp;        
    };
}
