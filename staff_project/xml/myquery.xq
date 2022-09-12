xquery version "1.0";

for $a in doc("/work/practica/staff/xml/Staff.xml")//Person
where $a/Emp_date > xs:date('2000-01-01')
return concat($a/../../@Name, ': ', $a/Name, ' ', $a/Surname, ' - ', $a/Emp_date, '
')
