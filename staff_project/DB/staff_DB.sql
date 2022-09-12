--
-- PostgreSQL database dump
--

-- Dumped from database version 9.6.6
-- Dumped by pg_dump version 9.6.6

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: Staff; Type: DATABASE; Schema: -; Owner: -
--

CREATE DATABASE "Staff" WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'ru_RU.UTF-8' LC_CTYPE = 'ru_RU.UTF-8';


\connect "Staff"

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: DATABASE "Staff"; Type: MAC LABEL; Schema: -; Owner: -
--

MAC LABEL ON DATABASE CURRENT_CATALOG IS '{0,0}';


--
-- Name: DATABASE "Staff"; Type: MAC CCR; Schema: -; Owner: -
--

MAC CCR ON DATABASE CURRENT_CATALOG IS ON;


--
-- Name: SCHEMA public; Type: MAC LABEL; Schema: -; Owner: -
--

MAC LABEL ON SCHEMA public IS '{0,0}';


--
-- Name: SCHEMA public; Type: MAC CCR; Schema: -; Owner: -
--

MAC CCR ON SCHEMA public IS ON;


--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: -
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = pg_catalog;

--
-- Name: EXTENSION plpgsql; Type: MAC LABEL; Schema: pg_catalog; Owner: -
--

MAC LABEL ON EXTENSION plpgsql IS '{0,0}';


--
-- Name: LANGUAGE plpgsql; Type: MAC LABEL; Schema: -; Owner: -
--

MAC LABEL ON LANGUAGE plpgsql IS '{0,0}';


SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: organization; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE organization (
    key character varying(38) NOT NULL,
    name character varying(30)
)
WITH (MACS=FALSE);


--
-- Name: TABLE organization; Type: MAC LABEL; Schema: public; Owner: -
--

MAC LABEL ON TABLE organization IS '{0,0}';


--
-- Name: TABLE organization; Type: MAC CCR; Schema: public; Owner: -
--

MAC CCR ON TABLE organization IS ON;


--
-- Name: personal; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE personal (
    key character varying(38) NOT NULL,
    name character varying(30),
    surname character varying(30),
    patronomic character varying(30),
    sex character varying(1),
    emp_date date,
    seniority character varying(30),
    org_key character varying(38) NOT NULL,
    sub_key character varying(38) NOT NULL,
    tab_id character varying(10)
)
WITH (MACS=FALSE);


--
-- Name: TABLE personal; Type: MAC LABEL; Schema: public; Owner: -
--

MAC LABEL ON TABLE personal IS '{0,0}';


--
-- Name: TABLE personal; Type: MAC CCR; Schema: public; Owner: -
--

MAC CCR ON TABLE personal IS ON;


--
-- Name: subdivizion; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE subdivizion (
    key character varying(38) NOT NULL,
    name character varying(30),
    org_key character varying(38) NOT NULL
)
WITH (MACS=FALSE);


--
-- Name: TABLE subdivizion; Type: MAC LABEL; Schema: public; Owner: -
--

MAC LABEL ON TABLE subdivizion IS '{0,0}';


--
-- Name: TABLE subdivizion; Type: MAC CCR; Schema: public; Owner: -
--

MAC CCR ON TABLE subdivizion IS ON;


--
-- Data for Name: organization; Type: TABLE DATA; Schema: public; Owner: -
--

COPY organization (key, name) FROM stdin;
{0449eac9-216e-40fc-8d30-1a31f8f00aee}	Mile a minute
{1134c2c7-6bd2-4055-a79d-b9e79bd5cdae}	Augsburger
\.


-- Obtained maclabel {0,0}
--
-- Data for Name: personal; Type: TABLE DATA; Schema: public; Owner: -
--

COPY personal (key, name, surname, patronomic, sex, emp_date, seniority, org_key, sub_key, tab_id) FROM stdin;
{a19a765b-84b8-4e7d-9dec-94bb2355481d}	ороро	ророр	роро	м	2000-01-01	22г 7м 19д	{0449eac9-216e-40fc-8d30-1a31f8f00aee}	{023db93a-2e9c-4611-a65d-c76602452dd9}	33434
{3c2c6fe8-3792-482e-8a2b-14ffcf91dbd8}	Олег	Петров	Иванович	м	1999-04-20	23г 4м 0д	{0449eac9-216e-40fc-8d30-1a31f8f00aee}	{3a98c821-b656-4b97-bee0-bee91822b6af}	888888
{6b951ee3-cf28-45ad-9616-2c66aed9209d}	Ирина	Иванова	Петровна	ж	2014-09-16	7г 11м 2д	{0449eac9-216e-40fc-8d30-1a31f8f00aee}	{3a98c821-b656-4b97-bee0-bee91822b6af}	999999
{9b147d46-c86b-487d-95fe-1cce69e5421d}	прпрпр	прпрпр	прпрпр	ж	2000-01-01	22г 7м 19д	{0449eac9-216e-40fc-8d30-1a31f8f00aee}	{5d271340-0a11-4846-a92c-0f34be191215}	131313
{dddc74f1-e4e5-43f7-a319-8ffbc8c49c1c}	Ирина	Волкова	Алексеевна	ж	2019-12-14	2г 8м 2д	{0449eac9-216e-40fc-8d30-1a31f8f00aee}	{5d271340-0a11-4846-a92c-0f34be191215}	666666
{f5f9c285-8461-495d-8a60-4901445fd5c5}	Мария	Сидорова	Александровна	ж	1997-08-13	25г 0м 5д	{0449eac9-216e-40fc-8d30-1a31f8f00aee}	{5d271340-0a11-4846-a92c-0f34be191215}	121212
{73d6fdd8-7320-4709-8670-bcd82ad6cb39}	Катерина	Разумовская	Андреевна	ж	2004-07-13	18г 1м 4д	{0449eac9-216e-40fc-8d30-1a31f8f00aee}	{707a677a-2210-4653-934c-e0d264e8a8f1}	777777
{744ff62e-8947-4d38-bbca-2766959f924d}	Станислав	Позин	Петрович	м	2020-10-24	1г 9м 22д	{0449eac9-216e-40fc-8d30-1a31f8f00aee}	{707a677a-2210-4653-934c-e0d264e8a8f1}	555555
{dbb78052-801e-45a5-806c-dbdcaec51ee7}	Андрей	Сидоров	Андреевич	м	2006-09-13	15г 11м 7д	{1134c2c7-6bd2-4055-a79d-b9e79bd5cdae}	{03cc9910-f1ed-4ec2-926f-a52a536a1a11}	333333
{3a68c74e-c44d-4f40-bc85-9d3eeea76d91}	Анна	Петрова	Сергеевна	ж	2017-03-26	5г 4м 20д	{1134c2c7-6bd2-4055-a79d-b9e79bd5cdae}	{26c071f6-0c40-4a44-84ef-b6407535b31a}	222222
{e42185cb-2bad-4127-9e49-2977fff4d7a3}	Иван	Иванов	Иванович	м	2012-02-03	10г 6м 13д	{1134c2c7-6bd2-4055-a79d-b9e79bd5cdae}	{26c071f6-0c40-4a44-84ef-b6407535b31a}	111111
{97e34258-93e5-4fba-bacc-85b4b7d8efed}	Михаил	Сергеев	Викторович	м	2010-08-30	11г 11м 20д	{1134c2c7-6bd2-4055-a79d-b9e79bd5cdae}	{80e88312-f6ae-456a-a63a-b506a82bbdb7}	444444
\.


-- Obtained maclabel {0,0}
--
-- Data for Name: subdivizion; Type: TABLE DATA; Schema: public; Owner: -
--

COPY subdivizion (key, name, org_key) FROM stdin;
{023db93a-2e9c-4611-a65d-c76602452dd9}	лдлдлд	{0449eac9-216e-40fc-8d30-1a31f8f00aee}
{3a98c821-b656-4b97-bee0-bee91822b6af}	Отдел-2	{0449eac9-216e-40fc-8d30-1a31f8f00aee}
{5d271340-0a11-4846-a92c-0f34be191215}	Отдел-3	{0449eac9-216e-40fc-8d30-1a31f8f00aee}
{707a677a-2210-4653-934c-e0d264e8a8f1}	Отдел-1	{0449eac9-216e-40fc-8d30-1a31f8f00aee}
{03cc9910-f1ed-4ec2-926f-a52a536a1a11}	Отдел-5	{1134c2c7-6bd2-4055-a79d-b9e79bd5cdae}
{26c071f6-0c40-4a44-84ef-b6407535b31a}	Отдел-4	{1134c2c7-6bd2-4055-a79d-b9e79bd5cdae}
{80e88312-f6ae-456a-a63a-b506a82bbdb7}	Отдел-6	{1134c2c7-6bd2-4055-a79d-b9e79bd5cdae}
\.


-- Obtained maclabel {0,0}
--
-- Name: organization organization_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY organization
    ADD CONSTRAINT organization_pkey PRIMARY KEY (key);


--
-- Name: personal personal_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY personal
    ADD CONSTRAINT personal_pkey PRIMARY KEY (key);


--
-- Name: subdivizion subdivizion_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY subdivizion
    ADD CONSTRAINT subdivizion_pkey PRIMARY KEY (key);


--
-- Name: fki_org_sub_fk; Type: INDEX; Schema: public; Owner: -
--

CREATE INDEX fki_org_sub_fk ON subdivizion USING btree (org_key);


--
-- Name: INDEX fki_org_sub_fk; Type: MAC LABEL; Schema: public; Owner: -
--

MAC LABEL ON INDEX fki_org_sub_fk IS '{0,0}';


--
-- Name: fki_per_org_fk; Type: INDEX; Schema: public; Owner: -
--

CREATE INDEX fki_per_org_fk ON personal USING btree (org_key);


--
-- Name: INDEX fki_per_org_fk; Type: MAC LABEL; Schema: public; Owner: -
--

MAC LABEL ON INDEX fki_per_org_fk IS '{0,0}';


--
-- Name: fki_per_sub_fk; Type: INDEX; Schema: public; Owner: -
--

CREATE INDEX fki_per_sub_fk ON personal USING btree (sub_key);


--
-- Name: INDEX fki_per_sub_fk; Type: MAC LABEL; Schema: public; Owner: -
--

MAC LABEL ON INDEX fki_per_sub_fk IS '{0,0}';


--
-- Name: subdivizion org_sub_fk; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY subdivizion
    ADD CONSTRAINT org_sub_fk FOREIGN KEY (org_key) REFERENCES organization(key) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: personal per_org_fk; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY personal
    ADD CONSTRAINT per_org_fk FOREIGN KEY (org_key) REFERENCES organization(key) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: personal per_sub_fk; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY personal
    ADD CONSTRAINT per_sub_fk FOREIGN KEY (sub_key) REFERENCES subdivizion(key) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

