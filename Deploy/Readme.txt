README (UTF-8, ff=dos)

For Mac OS X support, Library/Readme (Mac).txt might be helpful.

If VNR is corrupted and you would like to re-download VNR on Windows 7 or above,
you can simply execute the following script in Command Prompt:

    @powershell -NoProfile -ExecutionPolicy unrestricted -Command "iex ((new-object net.webclient).DownloadString('http://sakuradite.org/reader.ps1'))"

Alternatively, if VNR's server is down again, you can get a recent version at Google Drive:
    http://goo.gl/t31MqY
Or:
    https://drive.google.com/folderview?id=0B3YXxE6u-4bzc1RKWHpoLWZROTQ

如果VNR的服务里有down了，你也许可以在这里下载一个最近的版本：
    http://pan.baidu.com/s/1jGftD9W

如果你需要安装或重新安装新的VNR，那么直接把上面的那行@powershell开头的命令粘贴到Command Prompt里执行下就好了。
如果你暂时居住在大陆地区，那么最好使用这种方法来下载VNR。
因为在Google Code上的安装程序在某些地方会被屏蔽。
这个命令只对Windows 7/8有效，在Windows XP上无法工作。

RELEASE NOTE

Name:       Visual Novel Reader
Homepage:   http://sakuradite.com
Download:   http://sakuradite.com/reader.7z
License:    GNU GPL v3
Contact:    AnnotCloud@gmail.com

REQUIREMENTS

- Microsoft Windows XP SP3/Vista/7/8

Note:
- There is a bug that path to VNR cannot contains '#' or unicode characters such as
  Chinese or Japanese kanji or it will crash on startup ><

INSTALL
Not required.

UNINSTALL
Trash this folder, and delete the following:
- In explorer:  %APPDATA%/org.sakuradite.reader
- In regedit:   HKCU/Software/sakuradite.org/reader

DEBUG
- Library/Debug Reader: you can get the debug output of VNR
- Library/Debug Update: you can get the debug output of Update
- Library/Kill Reader: if VNR hangs, you can use this script to kill VNR

UPDATE
- Just launch Update.exe. Internet access is needed.

- If Update.exe does not work for you, please try "Update/Debug Update.cmd"

- If you have difficulties to get the apps by updating, please try this:
      http://sakuradite.org/reader.7z
  The files there are usually kind of outdated, though.

FAQ
- UAC of Vista/Windows 7/8
  Different from ITH, VNR usually does not need administrator privilege to work.
  But for games that are launched as admin, you need start VNR as admin as well.

  Other than that, it is NOT recommended to launch VNR as administrator.

  Under the water, giving VNR administrator privilege will allow it to access
  system-level processes. It is not safe, and it might drag down the performance
  of VNR when searching for running games.

ADDITIONAL SOFTWARE

VNR could be used alone to work with VN. But some advanced features requires
additional software to be installed.

Here's a checklist of useful apps that could enhance the functionality of VNR.
You can also find them in Preferences/Locations.

- ATLAS or LEC
  These apps are needed by offline Japanese-English translation.
  You can purchase them online from their website.

- Microsoft AppLocale or NTLEA
  VNR can use them to change Japanese locale of the game to your language.
  They are not required if you are using Japanese Windows.
  NTLEA comes together with VNR.
  AppLocale is free to download online.

- Microsoft Japanese IME (IMJP)
  By default, VNR uses MeCab to parse Japanese sentences.
  But the dictionary of MeCab is limited and out-of-dated.
  MS Japanese IME's large dictionary could help MeCab to parse Japanese to furigana.

- EPWING dictionary
  By default, VNR use EDICT to provide Japanese-English word translation.
  If you are using VNR to learn Japanese, a Japanese-Japanewse dictionary would
  be helpful. VNR supports using KOJIEN EPWING dictionary for JJ translation.
  The dictionary DVD can be purchased from Amazon.

  If you need support for other EPWING dictionaries, just let me know!

- Japanese text-to-speech (TTS)
  By default, VNR use Google TTS service to read Japanese, which requires
  Internet access. If you want offline service, extra TTS software is needed.
  There are several commercial Japanese TTS out there, such as VoiceText Misaki
  (female) or Show (male).

= CHINESE =

如果你在使用Mac，那么Library/Readme (Mac).txt也许可以帮上忙。

【系统要求】
- Microsoft Windows XP SP3/Vista/7/8

注：
- 如果你在用64位的Windows，VNR仍然需要上面的32位的msvc才可以运行 T_T
- VNR的路径不可以包含"#"和中文字符，不然启动时会崩溃的 ><

【安装】
不需要。

【卸载】
删除这个文件夹，然后清理下面的地方：
- 资源管理器：       %APPDATA%/org.sakuradite.reader
- 注册表(regedit）： HKCU/Software/sakuradite.org/reader

【调试】
- Library/Debug Reader: 你可以使用这个脚本来得到VNR的调试信息
- Library/Debug Update: 你可以使用这个脚本来得到更新的调试信息
- Library/Kill Reader: 如果VNR卡住了，你可以用这个脚本来杀掉VNR

【更新】
- 在有网络连接时，直接运行Update.exe就可以了。

- 如果Update.exe不能正常工作，可以尝试Update/Debug Update.cmd。

- 如果实在是不可以，你可以从这里下载：
    http://sakuradite.org/reader.7z
  不过那里的文件通常不是最新的版本。

【常见问题】
- 如果你无法启动VNR，通常是因为32位的msvc没有安装。
  运行Library/Debug Reader.cmd也许会帮助你找到问题的原因。
  如果实在无法启动，你也可以通过邮件可以向我抱怨！

- 关于UAC（用户权限），VNR和ITH不同，通常不需要管理员的权限来执行。
  但是如果要阅读的VN本身是用管理员权限打开的，那么只好用管理员权限来打开VNR。

  尽管如此，通常情况建议不要以管理员身份执行VNR。
  以管理员身份打开VNR，会使得VNR得以获得系统全部进程的信息。
  这样并不安全，而且会降低VNR搜索游戏进程的速度。

【中文化】
- 第一次运行后，在Preferences/Account中将Language调整为Chinese即可。

- 如果你来自台湾，可以在【使用偏好/翻译】中的最下方，选择开启简繁转换来将简体中
  文的翻译和字典转换为繁体。

- 关于简繁，除非明确标出【简体】，VNR中的【中文】指的是繁体中文。
  如果你能够看懂繁体中文，那么推荐不要使用简体中文。

  VNR可以替换游戏的文本为中文，但是由于日文程序多用SHIFT-JIS编码，这个编码同时支
  持日文和繁体中文的字符，但是不支持简体中文。如果翻译是简体中文，可能会造成文本
  渲染时变成"？？？"，甚至一句话在中间被截断。繁体中文的话，就不会有这个问题。

  另外，简体中文使得中国大陆自绝于世界上其他的汉字文化圈，所以并不推荐。

- 如果你来自中国大陆，那么请*不要*尝试开启Google TTS，不然VNR会变得很卡很卡的。

【额外的软件】
VNR可以单独使用。但是VNR自带的功能多为面向英文的使用者。
在英文开源社区中有很多对日文的支持，但是中文的就很少，且多为闭源或商用软件。
由于著作权原因，VNR不可以将这些商用软件打包一起发行，即使是免费的也不可以。
VNR一些重要的功能需要安装额外的软件才可以使用。

这里是一部分的软件的介绍，你也可以在【使用偏好】/【位置】中找到它们。

- JBeijing
  VNR默认使用在线的翻译服务。
  如果需要离线的日中翻译，要安装JBeijing才可以。

- Microsoft AppLocale和NTLEA
  如果使用的不是日文的Windows，那么很多日文游戏通常需要【转区】才可以运行。
  所谓【转区】就是说把游戏的日文的区域编码转换成中文。
  Microsoft的AppLocale和NTLEA可以实现转区。
  VNR已经自带了NTLEA。
  AppLocale可以在微软的官网上免费下载。

- 微软日文输入法（IMJP）
  VNR默认使用开源的MeCab来作为日文的分词器。
  微软日文输入法的词库可以用来帮助MeCab改善日文分词的质量。
  IMJP对有日文Windows或Office企业版的同学是免费的。
  安装完成后，建议免费更新下输入法的辞书。

- EPWING字典
  VNR默认使用开源的EDICT字典提供日英单词解释。
  如果要日中，或日日的单词翻译，需要安装额外的EPWING字典。
  所谓【EPWING】，是ISO标准的以CDROM为载体的日文电子书的格式。
  字典以文件夹的方式存贮，EPWING每个文件夹一定会有CATALOG或CATALOGS文件。
  VNR现在支持的字典包括広辞苑日日辞书，和小学馆的日中辞书。

  如果你需要其他EPWING字典的支持，可以发邮件给我。

- 日文语音合成（TTS）
  VNR默认使用Google TTS的在线服务来读出鼠标点击的日文单词和句子。
  如果你要在离线时使用，那么需要安装其他的日文TTS软件。
  这样的软件有很多，比如VoiceText的Misaki（女性）和Show（男性）。
  VoiceText是公认最棒的日文TTS，但是软件不是免费的。

EOF
