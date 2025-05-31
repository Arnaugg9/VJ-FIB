using System;

[Serializable]
public class ScoreEntry
{
    public string playerName;
    public int score;

    public ScoreEntry(string name, int s)
    {
        playerName = name;
        score = s;
    }
    public override string ToString()
    {
        return $"{playerName}: {score}";
    }
}