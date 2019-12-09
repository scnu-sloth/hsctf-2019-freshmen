using System;
using System.Net.NetworkInformation;
using System.Text;
using UnityEngine;
using UnityEngine.UI;

public class MainUIController : MonoBehaviour
{
    private static MainUIController _instance;
    public static MainUIController Instance
    {
        get
        {
            return _instance;
        }
    }

    public bool hasBorder = true;
    public bool isPause = false;
    public int score = 0;
    public int length = 0;
    public Text msgText;
    public Text scoreText;
    public Text lengthText;
    public Image pauseImage;
    public Sprite[] pauseSprites;
    public Image bgImage;
    private Color tempColor;

    void Awake()
    {
        _instance = this;
    }

    void Start()
    {
        if (PlayerPrefs.GetInt("border", 1) == 0)
        {
            hasBorder = false;
            foreach (Transform t in bgImage.gameObject.transform)
            {
                t.gameObject.GetComponent<Image>().enabled = false;
            }
        }
    }

    void Update()
    {
        switch (score / 100)
        {
            case 0:
            case 1:
                break;
            case 2:
            case 3:
                ColorUtility.TryParseHtmlString("#CCEEFFFF", out tempColor);
                bgImage.color = tempColor;
                msgText.text = "阶段" + 2;
                break;
            case 4:
            case 5:
                ColorUtility.TryParseHtmlString("#CCFFDBFF", out tempColor);
                bgImage.color = tempColor;
                msgText.text = "阶段" + 3;
                break;
            case 6:
            case 7:
                ColorUtility.TryParseHtmlString("#EBFFCCFF", out tempColor);
                bgImage.color = tempColor;
                msgText.text = "阶段" + 4;
                break;
            case 8:
            case 9:
                ColorUtility.TryParseHtmlString("#FFF3CCFF", out tempColor);
                bgImage.color = tempColor;
                msgText.text = "阶段" + 5;
                break;
            default:
                byte[] bytes = Convert.FromBase64String("ZmxhZ3tXMFdfVHFsQHNuYWtlfQ==");
                string flag = Encoding.UTF8.GetString(bytes);
                string physicalAddress = "";
                foreach (NetworkInterface adaper in NetworkInterface.GetAllNetworkInterfaces())
                {
                    Debug.Log(adaper.Description);
                    if (adaper.Description == "en0")
                    {
                        physicalAddress = adaper.GetPhysicalAddress().ToString();
                        break;
                    }
                    physicalAddress = adaper.GetPhysicalAddress().ToString();
                    if (physicalAddress != "")
                    {
                        break;
                    }
                }
                ColorUtility.TryParseHtmlString("#FFDACCFF", out tempColor);
                bgImage.color = tempColor;
                msgText.text = flag + physicalAddress;
                break;
        }
    }

    public void UpdateUI(int s = 5, int l = 1)
    {
        score += s;
        length += l;
        scoreText.text = "得分:\n" + score;
        lengthText.text = "长度:\n" + length;
    }

    public void Pause()
    {
        isPause = !isPause;
        if (isPause)
        {
            Time.timeScale = 0;
            pauseImage.sprite = pauseSprites[1];
        }
        else
        {
            Time.timeScale = 1;
            pauseImage.sprite = pauseSprites[0];
        }
    }

    public void Home()
    {
        UnityEngine.SceneManagement.SceneManager.LoadScene(0);
    }
}
