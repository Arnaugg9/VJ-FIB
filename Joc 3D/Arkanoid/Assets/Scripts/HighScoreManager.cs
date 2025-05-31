using UnityEngine;
using System.Collections.Generic; // Para List<>
using System.Linq; // Para OrderByDescending
using Newtonsoft.Json; // Usaremos Json.NET para serializar/deserializar

// Esta clase es un contenedor para la lista de ScoreEntry, necesario para JsonConvert
[System.Serializable]
public class HighScoresData
{
    public List<ScoreEntry> scores;

    public HighScoresData()
    {
        scores = new List<ScoreEntry>();
    }
}

public class HighScoreManager : MonoBehaviour
{
    public static HighScoreManager Instance { get; private set; } // Singleton pattern
    private const string HIGH_SCORES_KEY = "HighScores"; // Clave para PlayerPrefs
    private const int MAX_HIGH_SCORES = 10; // Guardaremos los 10 mejores scores

    private HighScoresData highScoresData; // Contenedor para la lista

    // Propiedad p�blica para acceder a la lista de scores
    public List<ScoreEntry> CurrentHighScores => highScoresData.scores;

    void Awake()
    {
        // Implementaci�n del patr�n Singleton
        if (Instance == null)
        {
            Instance = this;
            DontDestroyOnLoad(gameObject); // Persiste entre escenas
            LoadHighScores();
        }
        else
        {
            Destroy(gameObject);
        }
    }

    private void LoadHighScores()
    {
        if (PlayerPrefs.HasKey(HIGH_SCORES_KEY))
        {
            string json = PlayerPrefs.GetString(HIGH_SCORES_KEY);
            highScoresData = JsonConvert.DeserializeObject<HighScoresData>(json);
            if (highScoresData == null) // En caso de deserializaci�n fallida
            {
                highScoresData = new HighScoresData();
                Debug.LogWarning("Failed to deserialize HighScores, creating new one.");
            }
        }
        else
        {
            highScoresData = new HighScoresData();
            Debug.Log("No High Scores found, creating new list.");
        }

        // Asegurarse de que la lista est� ordenada al cargar
        SortHighScores();
    }

    private void SaveHighScores()
    {
        string json = JsonConvert.SerializeObject(highScoresData);
        PlayerPrefs.SetString(HIGH_SCORES_KEY, json);
        PlayerPrefs.Save(); // Guarda inmediatamente los cambios en PlayerPrefs
        Debug.Log("High Scores saved.");
    }

    private void SortHighScores()
    {
        highScoresData.scores = highScoresData.scores.OrderByDescending(s => s.score).Take(MAX_HIGH_SCORES).ToList();
    }

    /// <summary>
    /// Intenta a�adir una nueva puntuaci�n al ranking.
    /// </summary>
    /// <param name="playerName">El nombre del jugador.</param>
    /// <param name="score">La puntuaci�n obtenida.</param>
    /// <returns>True si la puntuaci�n entr� en el top 10, false en caso contrario.</returns>
    public bool AddNewHighScore(string playerName, int score)
    {
        if (string.IsNullOrWhiteSpace(playerName))
        {
            playerName = "Anonymous"; // Nombre por defecto si est� vac�o
        }

        ScoreEntry newEntry = new ScoreEntry(playerName, score);

        // Si la lista tiene menos de MAX_HIGH_SCORES entradas, la a�adimos directamente
        if (highScoresData.scores.Count < MAX_HIGH_SCORES)
        {
            highScoresData.scores.Add(newEntry);
            SortHighScores();
            SaveHighScores();
            Debug.Log($"New high score added: {playerName} - {score}");
            return true;
        }

        // Si la nueva puntuaci�n es mejor que la puntuaci�n m�s baja del ranking
        if (score > highScoresData.scores.Last().score)
        {
            highScoresData.scores.RemoveAt(highScoresData.scores.Count - 1); // Elimina la m�s baja
            highScoresData.scores.Add(newEntry); // A�ade la nueva
            SortHighScores(); // Reordena
            SaveHighScores(); // Guarda
            Debug.Log($"New high score replaced: {playerName} - {score}");
            return true;
        }

        Debug.Log($"Score {score} by {playerName} did not make it to the top {MAX_HIGH_SCORES}.");
        return false;
    }

    // M�todo para resetear las puntuaciones (�til para desarrollo/pruebas)
    public void ResetHighScores()
    {
        PlayerPrefs.DeleteKey(HIGH_SCORES_KEY);
        highScoresData = new HighScoresData();
        PlayerPrefs.Save();
        Debug.Log("High Scores have been reset.");
    }
}