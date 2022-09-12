<?xml version="1.0" encoding="UTF-8"?>
<!--Designed and generated by Altova StyleVision Enterprise Edition 2010 - see http://www.altova.com/stylevision for more information.-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:clitype="clitype" xmlns:fn="http://www.w3.org/2005/xpath-functions" xmlns:java="java" xmlns:link="http://www.xbrl.org/2003/linkbase" xmlns:xbrldi="http://xbrl.org/2006/xbrldi" xmlns:xbrli="http://www.xbrl.org/2003/instance" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:altova="http://www.altova.com" exclude-result-prefixes="clitype fn java link xbrldi xbrli xlink xs xsi altova">
	<xsl:output version="4.0" method="html" indent="no" encoding="UTF-8" doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" doctype-system="http://www.w3.org/TR/html4/loose.dtd"/>
	<xsl:param name="SV_OutputFormat" select="'HTML'"/>
	<xsl:variable name="XML" select="/"/>
	<xsl:template match="/">
		<html>
			<head>
				<title>
					<xsl:text> </xsl:text>
				</title>
				<style type="text/css">@page { margin-left:0.60in; margin-right:0.60in; margin-top:0.79in; margin-bottom:0.79in } @media print { br.altova-page-break { page-break-before: always; } }</style>
			</head>
			<body>
				<br/>
				<xsl:for-each select="$XML">
					<xsl:for-each select="Staff">
						<xsl:for-each select="Organization">
							<xsl:for-each select="@Name">
								<span style="font-size:larger; font-weight:700; text-align:center; "/>
								<xsl:value-of select="string(.)"/>
							</xsl:for-each>
							<br/>
							<table border="1">
								<xsl:variable name="altova:CurrContextGrid_06CB5AE8" select="."/>
								<thead>
									<tr style="height:0.39in; ">
										<th>
											<span style="text-align:center; ">
												<xsl:text>Табельный №</xsl:text>
											</span>
										</th>
										<th>
											<span style="text-align:center; ">
												<xsl:text>Фамилия</xsl:text>
											</span>
										</th>
										<th>
											<span style="text-align:center; ">
												<xsl:text>Имя</xsl:text>
											</span>
										</th>
										<th>
											<span style="text-align:center; ">
												<xsl:text>Отчество</xsl:text>
											</span>
										</th>
										<th>
											<span style="text-align:center; ">
												<xsl:text>Отдел</xsl:text>
											</span>
										</th>
										<th>
											<span style="text-align:center; ">
												<xsl:text>Пол</xsl:text>
											</span>
										</th>
										<th style="width:0.79in; ">
											<span style="text-align:center; ">
												<xsl:text>Дата начала отпуска </xsl:text>
											</span>
										</th>
										<th>
											<span style="text-align:center; ">
												<xsl:text>Дата окончания отпуска</xsl:text>
											</span>
										</th>
									</tr>
								</thead>
								<tbody style="bottom:auto; left:auto; right:auto; top:auto; ">
									<xsl:for-each select="Person">
										<tr style="text-align:center; ">
											<td>
												<xsl:for-each select="Tab_ID">
													<xsl:apply-templates/>
												</xsl:for-each>
											</td>
											<td>
												<xsl:for-each select="Surname">
													<xsl:apply-templates/>
												</xsl:for-each>
											</td>
											<td>
												<xsl:for-each select="Name">
													<xsl:apply-templates/>
												</xsl:for-each>
											</td>
											<td>
												<xsl:for-each select="Patronymic">
													<xsl:apply-templates/>
												</xsl:for-each>
											</td>
											<td>
												<xsl:for-each select="Subdivizion">
													<xsl:apply-templates/>
												</xsl:for-each>
											</td>
											<td>
												<xsl:for-each select="Sex">
													<xsl:apply-templates/>
												</xsl:for-each>
											</td>
											<td style="width:0.79in; ">
												<xsl:for-each select="Holidays">
													<xsl:for-each select="Begin_Date">
														<xsl:apply-templates/>
													</xsl:for-each>
												</xsl:for-each>
											</td>
											<td>
												<xsl:for-each select="Holidays">
													<xsl:for-each select="End_Date">
														<xsl:apply-templates/>
													</xsl:for-each>
												</xsl:for-each>
											</td>
										</tr>
									</xsl:for-each>
								</tbody>
							</table>
							<br/>
						</xsl:for-each>
					</xsl:for-each>
				</xsl:for-each>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
