![NEST](https://user-images.githubusercontent.com/52374179/73546680-faa76b00-4480-11ea-91bf-50d27eaddcac.png)
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

PacketMachine -Masayoshi Mizutani<br>
https://github.com/m-mizutani/packetmachine<br>

### セットアップ方法の例　(CentOS7の場合)
ターミナルで以下のコマンドを実行し，必要なライブラリをインストールします．
```
$ sudo yum install -y libpcap libpcap_devel 
```
また，<br>https://www.qt.io/download-qt-installer?hsCtaTracking=7ac7660e-f6f1-4d80-ae94-772be5615b6c|e1da64a4-2546-4fee-b31c-0c9d9eb85019<br>
からQtをインストールします．

## データベースのセットアップ
NESTはPostgreSQLを使用しているため，PostgreSQLのインストール，セッティングが必要です．<br>
### セッティング方法(CentOS７の場合)
以下のセッティング手順はhttps://qiita.com/mkyz08/items/519bd8d6a7140c8ed57e<br>
を参考にしています．ファイルの書き換える場所等がわからない場合はリンク先を参照してください．
1. PostgreSQLのインストール
PostgreSQL11のインストールのため，ターミナルで以下のコマンドを実行します．
```
# yum -y install https://download.postgresql.org/pub/repos/yum/reporpms/EL-7-x86_64/pgdg-redhat-repo-latest.noarch.rpm
# yum -y install postgresql11-server postgresql11-contrib
# yum -y install postgresql-devel
```

2. データベースクラスタの生成
/usr/lib/systemd/system/postgresql-11.serviceを以下のように書き換え，リロードします．
```
# vi /usr/lib/systemd/system/postgresql-11.service

− Environment=PGDATA=/var/lib/pgsql/11/data/
+ Environment=PGDATA=/data/

# systemctl daemon-reload
# PGSETUP_INITDB_OPTIONS="-E UTF8 --no-locale" /usr/pgsql-11/bin/postgresql-11-setup initdb
# cat /data/PG_VERSION //PostgreSQLのバージョンが 11であることを確認
11
```


3. パスの追加
起動前にファイルを書き換え，パスを追加します
```
# su - postgres

# vi /var/lib/pgsql/.pgsql_profile
+ PATH=/usr/pgsql-11/bin:$PATH
+ export PATH
```
書き換えたパスを読み込み，PostgreSQlをスタートします．
```
# source ~/.bash_profile
# systemctl start postgresql-11
```
4. ユーザとデータべースの作成
ユーザとデータベースを作成します．
```
$ createuser --login --pwprompt ユーザ名
Enter password for new role: 
Enter it again:

$ createdb --owner=オーナーとなるユーザ名 データベース名
```
5. 暗号化の有効化
以下のコマンドを実行し，拡張を有効化
```
postgres=# create extension pgcrypto;
```
作成したユーザをスーパーユーザにして拡張を有効化
```
postgres=# ALTER ROLE ユーザ名 SUPERUSER;
ユーザ名=# create extension pgcrypto;
```

6. ソフトウェアからの接続を許可
/data/postgresql.confと/data/pg_hba.confを書き換え，ソフトウェアからの接続を可能にします．
```
# vi /data/postgresql.conf

− #listen_addresses = 'localhost'
+ listen_addresses = '*'

# vi /data/pg_hba.conf
# "local" is for Unix domain socket connections only
− local all all ident
+ local all all trust
+ local db名　ユーザ名 md5
# IPv4 local connections:
− host all all 127.0.0.1/32 ident
+ host all all 127.0.0.1/32 trust
# IPv6 local connections:
− host all all ::1/128 ident
+ host all all ::1/128 trust
```
書き換えが完了したら，PostgreSQLをリスタートします
```
# systemctl restart postgresql.service
```
以上で設定は完了です．

## ビルドについて
ビルドはqmake,makeを用いる方法とQt付属のQtCreatorを用いる方法の2種類があります
### 1.qmake,makeを用いる方法
ターミナルでNESTに移動し，以下のコマンドを実行
```
$ qmake NEST.pro
$ make
```
するとビルドが完了し，実行ファイルが生成されます．<br>
### 2.QTCreatorを利用する方法
QTCreatorを開き，メニューバーのファイル→新しいプロジェクトを開く　からNEST.proを選択して開きます<br>
プロジェクトを開けたら，メニューバーのビルドからビルドすると，実行ファイルが生成されます．

## ロゴ
![LOGO](https://user-images.githubusercontent.com/52374179/73547018-84efcf00-4481-11ea-90fc-feb1ecf00464.png)
Designed by Kenta Takahashi
