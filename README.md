![NEST](https://user-images.githubusercontent.com/52374179/73546680-faa76b00-4480-11ea-91bf-50d27eaddcac.png)
## NEtwork Surveillance Tool
# パケットキャプチャを用いたネットワーク解析用GUIアプリケーション

## 動作環境
Linux系OS (パケットキャプチャのため管理者権限が必要)<br>
CentOS,Ubuntu,Mac OS など

## セットアップについて


以下のソフト、ライブラリのインストールが必要です<br>
1. libpcap<br>
2. Qt(Version 5.12.6にて動作確認済み)<br>
3. PostgreSQL(Version 11にて動作確認済み)<br>
4. packetmachine<br>
以下のリンクのREADMEのHow To Use を参考にインストールして下さい<br>

PacketMachine<br>
https://github.com/m-mizutani/packetmachine<br>

## ビルドについて
ビルドはqmake,makeを用いる方法とQt付属のQtCreatorを用いる方法の2種類があります
### 1.qmake,makeを用いる方法
ターミナルでNESTに移動し，以下のコマンドを実行
```
qmake NEST.pro
make
```
するとビルドが完了し，実行ファイルが生成されます．<br>
### 2.QTCreatorを利用する方法
QTCreatorを開き，メニューバーのファイル→新しいプロジェクトを開く　からNEST.proを選択して開きます<br>
プロジェクトを開けたら，メニューバーのビルドからビルドすると，実行ファイルが生成されます．

## ロゴ
![LOGO](https://user-images.githubusercontent.com/52374179/73547018-84efcf00-4481-11ea-90fc-feb1ecf00464.png)
