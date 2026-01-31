param(
    [Parameter(Mandatory=$true)]
    [string]$DlnaHost,

    [int]$Port = 50001,

    # Mit listázzunk?
    # Synology-nál tipikusan: 0 = root, 21 = Zene
    [string]$ObjectId = "21",

    [int]$RequestedCount = 50
)

# Force UTF-8 output in PowerShell console
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8

Add-Type -AssemblyName System.Web

$baseUrl    = "http://$DlnaHost`:$Port"
$controlUrl = "$baseUrl/ContentDirectory/control"

function BuildBrowseXml([string]$id, [int]$count) {
@"
<?xml version="1.0" encoding="utf-8"?>
<s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/"
            s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
 <s:Body>
  <u:Browse xmlns:u="urn:schemas-upnp-org:service:ContentDirectory:1">
   <ObjectID>$id</ObjectID>
   <BrowseFlag>BrowseDirectChildren</BrowseFlag>
   <Filter>*</Filter>
   <StartingIndex>0</StartingIndex>
   <RequestedCount>$count</RequestedCount>
   <SortCriteria></SortCriteria>
  </u:Browse>
 </s:Body>
</s:Envelope>
"@
}

# UTF-8 BOM nélkül
$Utf8NoBom = New-Object System.Text.UTF8Encoding($false)
$tmp = [System.IO.Path]::GetTempFileName()

try {
    [System.IO.File]::WriteAllText($tmp, (BuildBrowseXml $ObjectId $RequestedCount), $Utf8NoBom)

    # curl futtatás CMD-n keresztül (stabil)
$cmd = @(
    'type "' + $tmp + '" |',
    'curl -s',
    '-H "SOAPACTION: \"urn:schemas-upnp-org:service:ContentDirectory:1#Browse\""',
    '-H "Content-Type: text/xml; charset=utf-8"',
    '--data-binary @-',
    '"' + $controlUrl + '"'
) -join ' '

    $soapText = (& cmd.exe /c $cmd 2>&1) -join "`n"
    if (-not $soapText) {
        Write-Host "ERROR: empty response"
        exit 1
    }

    # SOAP -> Result (HTML-escaped DIDL) -> HTML decode
    try { [xml]$soap = $soapText } catch {
        Write-Host "ERROR: response is not valid XML (SOAP). First 10 lines:"
        ($soapText -split "`n" | Select-Object -First 10) | ForEach-Object { Write-Host $_ }
        exit 1
    }

    $resultNode = $soap.SelectSingleNode("//*[local-name()='BrowseResponse']/*[local-name()='Result']")
    if (-not $resultNode -or -not $resultNode.InnerText) {
        Write-Host "ERROR: SOAP has no BrowseResponse/Result. First 10 lines:"
        ($soapText -split "`n" | Select-Object -First 10) | ForEach-Object { Write-Host $_ }
        exit 1
    }

    $didlText = [System.Web.HttpUtility]::HtmlDecode($resultNode.InnerText)

    # Container lista (ID + Title)
    $rx = New-Object System.Text.RegularExpressions.Regex(
        "<container\b[^>]*\bid=""([^""]+)""[^>]*>.*?<dc:title>(.*?)</dc:title>",
        [System.Text.RegularExpressions.RegexOptions]::Singleline
    )

    $matches = $rx.Matches($didlText)
    if ($matches.Count -eq 0) {
        Write-Host "No containers found under ObjectID=$ObjectId"
        exit 0
    }

    Write-Host ""
    Write-Host ("DLNA container list under ObjectID={0} on {1}" -f $ObjectId, $DlnaHost)
    Write-Host "----------------------------------------"

    foreach ($m in $matches) {
        $id = [System.Web.HttpUtility]::HtmlDecode($m.Groups[1].Value)
        $title = [System.Web.HttpUtility]::HtmlDecode($m.Groups[2].Value)
        Write-Host ("{0}`t{1}" -f $id, $title)
    }

} finally {
    Remove-Item $tmp -ErrorAction SilentlyContinue
}
